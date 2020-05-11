#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon = QIcon("/home/vladislav/Рабочий стол/CourseWork/HardCodeIDE/icon.png");
    this->setWindowIcon(icon);
    this->setWindowTitle("Start of work");

    codeEditorForm = new CodeEditor();
    connect(this, SIGNAL(SendFolderPath(QString)), codeEditorForm, SLOT(RecieveFolderPath(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_create_project_button_clicked()
{

}

void MainWindow::on_open_project_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        codeEditorForm->show();
        emit SendFolderPath(dir);
        this->close();
    }
}
