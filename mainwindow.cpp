#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listchecker.h"

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
    QString link = ui->linkLineEdit->text();
    ListChecker listChecker;

    if (listChecker.checkIfListItemExist(link, ui->linkListWidget)){
        //TODO
        return;
     }
     ui->linkListWidget->addItem(link);
}
