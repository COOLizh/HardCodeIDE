#include "createprojectform.h"
#include "ui_createprojectform.h"
#include <QFileDialog>

createProjectForm::createProjectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createProjectForm)
{
    ui->setupUi(this);
    ui->folder_path_edit->setReadOnly(true);
    QIcon icon = QIcon("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/icon.png");
    this->setWindowIcon(icon);
    this->setWindowTitle("Create project");
    codeEditorForm = new CodeEditor();
    connect(this, SIGNAL(SendFolderPath(QString)), codeEditorForm, SLOT(RecieveFolderPath(QString)));
}

createProjectForm::~createProjectForm()
{
    delete ui;
}

void createProjectForm::on_overview_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    ui->folder_path_edit->setText(dir);
}

void createProjectForm::on_create_project_button_clicked()
{
    if(!isCorrectProjectName(ui->project_name_edit->text()))
    {
        ui->error_label->setText("Invalid name of project");
        return;
    }
    if(ui->folder_path_edit->text() == "" && ui->project_name_edit->text() == "")
    {
      ui->error_label->setText("Not all data inputed");
      return;
    }

    QDir dir1(ui->folder_path_edit->text());
    dir1.mkdir(ui->project_name_edit->text());
    QString dir = ui->folder_path_edit->text();
    dir += "/" + ui->project_name_edit->text();
    QString tmp = dir + "/main.cpp";
    QFile file(tmp);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        file.write("//*write your code ;)*//");
        file.close();
    }
    emit SendFolderPath(dir);
    codeEditorForm->show();
    this->close();
}

bool createProjectForm::isCorrectProjectName(QString name)
{
    for(auto &c : name)
        if(c == ' ')
            return false;

    return true;
}
