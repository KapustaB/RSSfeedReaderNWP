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
#include <QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->linksTreeWidget->setColumnCount(1);
    ui->linksTreeWidget->header()->close();
    QList<QList<RssFeed>> rssFeeds;
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

    if(listChecker.checkIfListItemExist(link, ui->linksTreeWidget)){
        makeNewMsgBoxLinkExists();
        return;
     }
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

    if (httpStatusCode >= 200 && httpStatusCode < 300)
        xmlReader->readXmlData(reply);
    else if (httpStatusCode >= 300 && httpStatusCode < 400)
        xmlReader->redirectReply(reply);

    this->articleList = xmlReader->getArticleList();
    rssFeeds.append(this->articleList);
    if(articleList.isEmpty()) return;

    ListChecker listchecker;
    for(int i=0; i<articleList.size(); ++i){
        RssFeed feed = articleList[i];
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,feed.getTitle());
        if(!listchecker.checkIfListItemExist(feed.getTitle(), ui->linksTreeWidget))
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
        qDebug() << "Cannot open file for writing: " << qPrintable(file.errorString());
        return;
    }
    QTextStream out(&file);
    out.setFieldWidth(10);
    out << txt << endl;
}

void MainWindow::print(QPrinter *printer)
{
    QPainter painter(printer);
    painter.setFont(QFont("Arial", 12));
    QRect totalRect = printer->pageRect();
    painter.drawText( QRect(100, 100, totalRect.width()-200 , totalRect.height()-200 ) ,
                     Qt::AlignLeft | Qt::TextJustificationForced | Qt::TextIncludeTrailingSpaces
                   | Qt::TextExpandTabs | Qt::TextWordWrap| Qt::TextWrapAnywhere,
                     ui->msgFeedTextBrowser->toPlainText() );
}

void MainWindow::on_printFeedBtn_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
    preview.exec();
}

void MainWindow::on_linksTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->msgFeedTextBrowser->clear();
    ui->pubDateLabel->clear();
    QString selectedItemTxt = ui->linksTreeWidget->currentItem()->text(column);

    int index = ui->linksTreeWidget->currentIndex().row();
    int listItemIndex = ui->linksTreeWidget->indexOfTopLevelItem(item->parent());

    if(item->parent()){
        ui->msgFeedTextBrowser->append(rssFeeds[listItemIndex][index].getTitle());
        ui->msgFeedTextBrowser->append("\n");
        ui->msgFeedTextBrowser->append(rssFeeds[listItemIndex][index].getDescription());
        ui->pubDateLabel->setText(rssFeeds[listItemIndex][index].getPubDate());
        return;
    }
     foreach(auto i, item->takeChildren()) delete i;
     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
     connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));
     manager->get(QNetworkRequest(QUrl(selectedItemTxt)));
}

void MainWindow::on_linksTreeWidget_clicked(const QModelIndex &index)
{
     ui->removeLinkBtn->setEnabled(true);
}
