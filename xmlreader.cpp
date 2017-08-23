#include "xmlreader.h"
#include <QDebug>
#include <QXmlStreamReader>

XmlReader::XmlReader()
{

}

void XmlReader::readXmlData(QNetworkReply *reply){

   QList<QString> elementList = {"titile", "description", "category", "pubDate"};
   qDebug() << "Reading xml data";

   QByteArray bytes = reply->readAll();
   QXmlStreamReader reader;
   reader.addData(bytes);
   while(!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            qDebug()<<reader.name();
            qDebug()<<elementList[1];
            for(int i=0; i<elementList.size(); ++i){
                if(reader.name() == elementList[i]) sendText(reader.readElementText());
            }
        }
   }
   if (reader.hasError()) {
         qDebug() << reader.errorString();
   }
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
