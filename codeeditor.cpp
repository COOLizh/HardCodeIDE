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
        if(it.fileName() == ".." || it.fileName().isEmpty() || it.fileName() == ".")
        {
            it.next();
            continue;
        }
        addTreeChild(it.fileName(),it.filePath(), it.fileInfo().isDir());
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
    dirsBack++;
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

