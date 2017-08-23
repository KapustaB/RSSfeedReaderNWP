#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listchecker.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include "xmlreader.h"

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

void  MainWindow::updateText(const QString& newText)
{
  ui->msgFeedTextBrowser->append(newText);
}

void MainWindow::requestReceived(QNetworkReply *reply)
{
    XmlReader *xmlReader = new XmlReader();
    connect(xmlReader, SIGNAL(sendText(const QString&)), this, SLOT(updateText(const QString&)));

    reply->deleteLater();

    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        return;
    }

    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << httpStatusCode;

    if (httpStatusCode >= 200 && httpStatusCode < 300)
        xmlReader->readXmlData(reply);
    else if (httpStatusCode >= 300 && httpStatusCode < 400)
        xmlReader->redirectReply(reply);


    qDebug() << "Gotov";
    reply->manager()->deleteLater();
}

void MainWindow::on_linkListWidget_doubleClicked(const QModelIndex &index)
{
    ui->msgFeedTextBrowser->clear();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));
    qDebug() << "manager->get";
    manager->get(QNetworkRequest(QUrl(ui->linkListWidget->item(index.row())->text())));
}
