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
    QString text;
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
    parent = treeItem;
}

void CodeEditor::addTreeChild(QString name, QString currentPath)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
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

