#include "xmlreader.h"
#include <QDebug>
#include <QXmlStreamReader>
#include "rssfeed.h"

XmlReader::XmlReader()
{

}
XmlReader::~XmlReader(){

}

void XmlReader::readXmlData(QNetworkReply *reply){

   QList<QString> elementList = {"title", "description","pubDate"};
   RssFeed feed;
   QByteArray bytes = reply->readAll();
   QXmlStreamReader reader;
   reader.addData(bytes);
   while(!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
              QString elName = reader.name().toString();
              if(elName == elementList[0])
                      feed.setTitle(reader.readElementText());
              else if (elName == elementList[1])
                      feed.setDescription(reader.readElementText());
              else if (elName == elementList[2])
                      feed.setPubDate(reader.readElementText());
              }

        if(!feed.getTitle().isEmpty() && !feed.getDescription().isEmpty() && !feed.getPubDate().isEmpty()) {
            qDebug()<< feed.getPubDate();
            if((std::find_if(std::begin(articleList), std::end(articleList),
                             [&](RssFeed f){ return (f.getTitle() == feed.getTitle() ||
                                                     f.getDescription() == feed.getDescription() ||
                                                     f.getPubDate() == feed.getPubDate()); }) == articleList.end()))
            {
                articleList.append(feed);
                qDebug()<<feed.getTitle();
            }
        }
   }
   if (reader.hasError()) qDebug() << reader.errorString();
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
    return;
}
