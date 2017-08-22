#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listchecker.h"
#include <QMessageBox>

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

void  makeNewMsgBoxLinkExists(){
    QMessageBox::information(0, "Error", "Link already exists!");
}

void MainWindow::on_addLinkBtn_clicked()
{
    QString link = ui->linkLineEdit->text();
    ListChecker listChecker;

    if (listChecker.checkIfListItemExist(link, ui->linkListWidget)){
        makeNewMsgBoxLinkExists();
        return;
     }
     ui->linkListWidget->addItem(link);
}

void MainWindow::on_linkListWidget_itemClicked(QListWidgetItem *item)
{
    ui->removeLinkBtn->setEnabled(true);
}

void MainWindow::on_removeLinkBtn_clicked()
{
    delete ui->linkListWidget->currentItem();
}

void MainWindow::on_removeLinkBtn_clicked(bool checked)
{
    if (checked == false) ui->removeLinkBtn->setEnabled(false);
}
