#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QDirIterator>
#include <QTreeWidget>
#include <qtermwidget5/qtermwidget.h>
#include <QFile>
#include "highlighter.h"
#include "textedit.h"
#include <QAbstractItemModel>
#include <QStringListModel>

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

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::CodeEditor *ui;
    QString folderPath, parentPath;
    QTreeWidgetItem *parent;
    QTermWidget *console;
    Highlighter *highlighter;
    QCompleter *completer = nullptr;
    TextEdit *completingTextEdit;
    QAbstractItemModel *modelFromFile(const QString& fileName);
    void addTreeRoot(QString name);
    void addTreeChild(QString name, QString currentPath, bool isDir);
    bool isChild(QString currentPath);
    int createParentOfParentPath(QString currentPath);
    int checkFileType(QString name);
    void createDirTree(QString dir);
};

#endif // CODEEDITOR_H
