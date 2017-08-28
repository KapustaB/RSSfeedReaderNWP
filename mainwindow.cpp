#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listchecker.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include "xmlreader.h"
#include "rssfeed.h"
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->linksTreeWidget->setColumnCount(1);
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

     QTreeWidgetItem *item = new QTreeWidgetItem(ui->linksTreeWidget);
     item->setText(0,link);
     ui->linksTreeWidget->addTopLevelItem(item);
     ui->msgFeedTextBrowser->clear();
}


void MainWindow::on_removeLinkBtn_clicked()
{
  delete ui->linksTreeWidget->currentItem();
}

void MainWindow::on_removeLinkBtn_clicked(bool checked)
{
    if (checked == false) ui->removeLinkBtn->setEnabled(false);
}

void MainWindow::requestReceived(QNetworkReply *reply)
{
    XmlReader *xmlReader = new XmlReader();

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

    this->articleList = xmlReader->getArticleList();
    if(articleList.isEmpty()) return;

    for(int i=0;i<articleList.size();++i){
        RssFeed feed = articleList[i];
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,feed.getTitle());
        ui->linksTreeWidget->currentItem()->addChild(item);
    }
    reply->manager()->deleteLater();
}


void MainWindow::on_saveFeedBtn_clicked()
{
    QTextDocument *doc = ui->msgFeedTextBrowser->document();
    QString txt = doc->toRawText();

    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save File"),"RSSfeed.txt", QObject::tr("text file (*.txt)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing: "
                  << qPrintable(file.errorString());
        return;
    }
    QTextStream out(&file);
    out << txt << endl;
}

void MainWindow::on_printFeedBtn_clicked()
{

}

void MainWindow::on_linksTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->msgFeedTextBrowser->clear();
    QString selectedItemTxt = ui->linksTreeWidget->currentItem()->text(column);
    qDebug()<<selectedItemTxt;
    //Top item - parent, ako nije - child
    if(item->parent()){
        for(int j=0; j<articleList.size(); ++j){
        if(articleList[j].getTitle()== selectedItemTxt){
            qDebug()<<articleList[j].getDescription();
            ui->msgFeedTextBrowser->append(articleList[j].getDescription());
        }
    }
}
     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
     connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));
     manager->get(QNetworkRequest(QUrl(selectedItemTxt)));
}

void MainWindow::on_linksTreeWidget_clicked(const QModelIndex &index)
{
     ui->removeLinkBtn->setEnabled(true);
}
