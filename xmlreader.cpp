#include "xmlreader.h"
#include <QDebug>
#include <QXmlStreamReader>
#include "rssfeed.h"

XmlReader::XmlReader()
{
  QList<RssFeed> articleList;
}


void XmlReader::readXmlData(QNetworkReply *reply){

   QList<QString> elementList = {"title", "description","pubDate"};
   qDebug() << "Reading xml data";
    RssFeed *feed = new RssFeed();
   QByteArray bytes = reply->readAll();
   QXmlStreamReader reader;
   reader.addData(bytes);
   while(!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
              if(reader.name().toString()==elementList[0])
                      feed->setTitle(reader.readElementText());
              else if (reader.name().toString()==elementList[1])
                      feed->setDescription(reader.readElementText());
              else if (reader.name().toString()==elementList[2])
                      feed->setPubDate(reader.readElementText());
              }

        if(!feed->getTitle().isEmpty() && !feed->getDescription().isEmpty()) {
            if(checkIfInList(feed->getTitle(),feed->getDescription())){
                articleList.append(*feed);
                RssFeed *feed = new RssFeed();
            }
        }
   }
   if (reader.hasError()) {
         qDebug() << reader.errorString();
   }
}
int XmlReader::checkIfInList(QString title, QString desc){
    for(int i=0; i<articleList.size();++i){
        if(articleList[i].getTitle()==title)return 0;
        else if (articleList[i].getDescription()==desc) return 0;
    }
    return 1;
}

QList<RssFeed> XmlReader::getArticleList(){
    return this->articleList;
}

void XmlReader::redirectReply(QNetworkReply *reply){

    QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    newUrl = reply->url().resolved(newUrl);

    QNetworkAccessManager *manager = reply->manager();
    QNetworkRequest redirection(newUrl);
    QNetworkReply *newReply = manager->get(redirection);
    this->readXmlData(newReply);
    return; // to keep the manager for the next request
}
