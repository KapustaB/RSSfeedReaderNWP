#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listchecker.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>


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

void MainWindow::requestReceived(QNetworkReply *reply)
{
    reply->deleteLater();
    qDebug() << (reply->error() == QNetworkReply::NoError);
    if(reply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << v;
        if (v >= 200 && v < 300) // Success
        {
            qDebug() << "Reading data";
            QByteArray bytes = reply->readAll();
            QString str = QString::fromUtf8(bytes);
            QXmlStreamReader reader;
            reader.addData(bytes);
            while(!reader.atEnd()) {
                 reader.readNext();
                 if (reader.isStartElement()) {
                     if (reader.name() == "title") {
                        ui->msgFeedTextBrowser->append("____");
                         ui->msgFeedTextBrowser->append(reader.readElementText());
                         /*
                         foreach(const QXmlStreamAttribute &attr, reader.attributes()) {

                             qDebug()<< "**" <<attr.name().toString();
                             if (attr.name().toString() == QLatin1String("title")) {
                                 QString attribute_value = attr.value().toString();
                                 qDebug() << attribute_value;
                             }
                         } */
                     }
                     if (reader.name()=="description"){
                         ui->msgFeedTextBrowser->append(reader.readElementText());
                     }
                     if(reader.name()=="category"){
                         ui->msgFeedTextBrowser->append(reader.readElementText());
                     }

                 }
            }
            if (reader.hasError()) {
                  qDebug() << "ERROR!";
                  qDebug() << reader.errorString();
            }

            qDebug() << str;
            qDebug() << bytes;

        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative,
            // we have to use the previous one to resolve it
            newUrl = reply->url().resolved(newUrl);

            QNetworkAccessManager *manager = reply->manager();
            QNetworkRequest redirection(newUrl);
            QNetworkReply *newReply = manager->get(redirection);
            requestReceived(newReply);
            return; // to keep the manager for the next request
        }
    }
    else
    {
        // Error
        qDebug() << reply->errorString();
    }
    qDebug() << "NO";
    reply->manager()->deleteLater();
}

void MainWindow::on_linkListWidget_doubleClicked(const QModelIndex &index)
{
    ui->msgFeedTextBrowser->clear();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestReceived(QNetworkReply*)));
    qDebug() << "manager->get";
    manager->get(QNetworkRequest(QUrl(ui->linkListWidget->item(index.row())->text())));
}
