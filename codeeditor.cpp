#include "codeeditor.h"
#include "ui_codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEditor)
{
    ui->setupUi(this);
    QIcon icon = QIcon("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/icon.png");
    this->setWindowIcon(icon);
    this->setWindowTitle("HardCodeIDE");
}

CodeEditor::~CodeEditor()
{
    delete ui;
}

void CodeEditor::RecieveFolderPath(QString dir)
{
    folderPath = dir;
    ui->treeWidget->setHeaderLabel("Project");
    ui->treeWidget->setSortingEnabled(false);
    ui->treeWidget->setColumnCount(1);
    QDirIterator it(folderPath, QDirIterator::Subdirectories);
    QDir *currentDir = new QDir();
    parentPath = folderPath;
    currentDir->setPath(folderPath);
    addTreeRoot(currentDir->dirName());
    while (it.hasNext()) {
        if(it.fileName() == ".." || it.fileName().isEmpty() || it.fileName() == ".")
        {
            it.next();
            continue;
        }
        addTreeChild(it.fileName(),it.filePath());
        it.next();
    }
}

void CodeEditor::addTreeRoot(QString name)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, name);
    treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/folder.png"));
    parent = treeItem;
}

void CodeEditor::addTreeChild(QString name, QString currentPath)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
    switch (checkFileType(name)) {
    case 0:
        treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/go.jpg"));
        break;
    case 1:
        treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/plus.png"));
        break;
    default:
        if(name.contains('.'))
        {
            treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/file.png"));
        }
        else
        {
            treeItem->setIcon(0, QPixmap("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/folder.png"));
        }
    }
    if(!isChild(currentPath))
    {
        int dirsBack = createParentOfParentPath(currentPath);
        dirsBack++;
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
    return true;
}

int CodeEditor::createParentOfParentPath(QString currentPath)
{
    QString tmp;
    int dirsBack = 0;
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
    int l = name.length();
    if(l > 3 && name[l - 1] == 'o' && name[l - 2] == 'g' && name[l - 3] == '.')
        return 0;
    if(l > 4 && name[l - 1] == 'p' && name[l - 2] == 'p' && name[l - 3] == 'c' && name[l - 4] == '.')
        return 1;
    return 2;
}

