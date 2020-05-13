#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include "codeeditor.h"
#include "createprojectform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_create_project_button_clicked();

    void on_open_project_button_clicked();

signals:
    void SendFolderPath(QString path);

private:
    Ui::MainWindow *ui;
    CodeEditor *codeEditorForm;
    createProjectForm *newProjectForm;
};
#endif // MAINWINDOW_H
