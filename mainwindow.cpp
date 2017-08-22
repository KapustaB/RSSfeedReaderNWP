#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addLinkBtn_clicked()
{
    QString link;
    link = ui->linkLineEdit->text();
    ui->linkListWidget->addItem(link);
}
