#include "codeeditor.h"
#include "ui_codeeditor.h"
//#include <QTextStream>
//QTextStream cout(stdout);
//QTextStream cin(stdin);


CodeEditor::CodeEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEditor)
{
    ui->setupUi(this);
    QIcon icon = QIcon("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/icon.png");
    this->setWindowIcon(icon);
    this->setWindowTitle("HardCodeIDE");

    completingTextEdit = new TextEdit(this);
    completer = new QCompleter(this);
    completer->setModel(modelFromFile("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/cppkeywords.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completingTextEdit->setCompleter(completer);
    completingTextEdit->setGeometry(240, 20, 1681, 791);
    completingTextEdit->setEnabled(false);

    QIcon::setThemeName(QStringLiteral("oxygen"));
    console = new QTermWidget(0, this);
    QFont font = QApplication::font();
    font.setFamily(QStringLiteral("Monospace"));
    font.setPointSize(12);
    console->setTerminalFont(font);
    console->setScrollBarPosition(QTermWidget::ScrollBarLeft);
    const auto arguments = QApplication::arguments();
    for (const QString& arg : arguments)
    {
        if (console->availableColorSchemes().contains(arg))
            console->setColorScheme(arg);
        if (console->availableKeyBindings().contains(arg))
            console->setKeyBindings(arg);
    }
    console->setGeometry(0, 812, 1920, 1080);

    font.setFamily(QStringLiteral("Nyala"));
    font.setPointSize(15);
    font.setBold(false);
    font.setItalic(false);
    font.setWeight(50);
    completingTextEdit->setFont(font);
    completingTextEdit->setStyleSheet(QStringLiteral("font: 12pt \"Nyala\";"));
}

CodeEditor::~CodeEditor()
{
    delete ui;
}

void CodeEditor::RecieveFolderPath(QString dir)
{
    folderPath = dir;
    console->setWorkingDirectory(folderPath);
    console->startShellProgram();
    createDirTree(folderPath);
}

void CodeEditor::createDirTree(QString dir)
{
    ui->treeWidget->setHeaderLabel("Project");
    ui->treeWidget->setSortingEnabled(false);
    ui->treeWidget->setColumnCount(1);
    QDirIterator it(dir, QDirIterator::Subdirectories);
    QDir *currentDir = new QDir();
    parentPath = dir;
    currentDir->setPath(dir);
    addTreeRoot(currentDir->dirName());
    while (it.hasNext()) {
        it.next();
        if(it.fileName() == ".." || it.fileName() == ".")
            continue;
        addTreeChild(it.fileName(),it.filePath(), it.fileInfo().isDir());
    }
}

void CodeEditor::addTreeRoot(QString name)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, name);
    treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/folder.png"));
    parent = treeItem;
}

void CodeEditor::addTreeChild(QString name, QString currentPath, bool isDir)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
    if(isDir)
        treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/folder.png"));
    else
    {
        switch (checkFileType(name)) {
        case 0:
            treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/go.jpg"));
            break;
        case 1:
            treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/plus.png"));
            break;
        default:
            treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/file.png"));
        }
    }

    if(!isChild(currentPath))
    {
        int dirsBack = createParentOfParentPath(currentPath);
        while(dirsBack--)
            parent = parent->parent();

    }
    parent->addChild(treeItem);
    parent = treeItem;
    parentPath = currentPath;
}

bool CodeEditor::isChild(QString currentPath)
{
    for(int i = 0; i < parentPath.length(); i++)
        if(parentPath[i] != currentPath[i])
            return false;
    if(parentPath.length() < currentPath.length() && currentPath[parentPath.length()] != '/')
        return false;
    return true;
}

int CodeEditor::createParentOfParentPath(QString currentPath)
{
    QString tmp;
    int dirsBack = 1;
    for(int i = 0; i < parentPath.length(); i++)
    {
        if(currentPath[i] != parentPath[i])
        {
            for(int j = i; j < parentPath.length(); j++)
                if(parentPath[j] == '/')
                    dirsBack++;
            break;
        }
        tmp += currentPath[i];
    }
    tmp.remove(tmp.length() - 1, 1);
    parentPath = tmp;
    return dirsBack;
}

int CodeEditor::checkFileType(QString name)
{
    if(name.contains(".go"))
        return 0;
    if(name.contains(".cpp"))
        return 1;
    return 2;
}


void CodeEditor::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->label->setText(item->text(column));
    QString pathToSelectedItem = item->text(column);
    pathToSelectedItem = "/" + pathToSelectedItem;
    while(item->parent() != nullptr)
    {
        item = item->parent();
        if(item->parent() == nullptr)
            break;
        pathToSelectedItem = item->text(column) + pathToSelectedItem;
        pathToSelectedItem = "/" + pathToSelectedItem;
    }
    pathToSelectedItem = folderPath + pathToSelectedItem;
    QFile file(pathToSelectedItem);
    QTextOption textOptions = completingTextEdit->document()->defaultTextOption();
    textOptions.setTabStop(20);
    completingTextEdit->document()->setDefaultTextOption(textOptions);
    highlighter = new Highlighter(completingTextEdit->document());
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        completingTextEdit->setPlainText(file.readAll());
        currentFilePath = pathToSelectedItem;
        completingTextEdit->setEnabled(true);
    }
}

QAbstractItemModel *CodeEditor::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << QString::fromUtf8(line.trimmed());
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}


void CodeEditor::on_save_button_clicked()
{
    QString str1 = completingTextEdit->toPlainText();
    QByteArray ba = str1.toLocal8Bit();
    const char *c_str2 = ba.data();
    QFile file(currentFilePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::WriteOnly)) {
        file.write(c_str2);
        file.close();
    }
}

void CodeEditor::on_update_tree_button_clicked()
{
    ui->treeWidget->clear();
    createDirTree(folderPath);
}

void CodeEditor::on_open_project_button_clicked()
{
    on_save_button_clicked();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        ui->treeWidget->clear();
        RecieveFolderPath(dir);
    }

}
