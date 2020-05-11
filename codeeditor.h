#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QDirIterator>
#include <QTreeWidget>

namespace Ui {
class CodeEditor;
}

class CodeEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

public slots:
    void RecieveFolderPath(QString dir);

private:
    Ui::CodeEditor *ui;
    QString folderPath, parentPath;
    QTreeWidgetItem *parent;
    void addTreeRoot(QString name);
    void addTreeChild(QString name, QString currentPath);
    bool isChild(QString currentPath);
    int createParentOfParentPath(QString currentPath);
};

#endif // CODEEDITOR_H
