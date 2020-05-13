#ifndef CREATEPROJECTFORM_H
#define CREATEPROJECTFORM_H

#include <QWidget>
#include <QLayout>
#include <QFile>
#include "codeeditor.h"

namespace Ui {
class createProjectForm;
}

class createProjectForm : public QWidget
{
    Q_OBJECT

public:
    explicit createProjectForm(QWidget *parent = nullptr);
    ~createProjectForm();

private slots:
    void on_overview_button_clicked();

    void on_create_project_button_clicked();

signals:
    void SendFolderPath(QString path);

private:
    Ui::createProjectForm *ui;
    CodeEditor *codeEditorForm;
    bool isCorrectProjectName(QString name);
};

#endif // CREATEPROJECTFORM_H
