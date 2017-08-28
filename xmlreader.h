#ifndef XMLREADER_H
#define XMLREADER_H
#include <QNetworkReply>
#include <QMainWindow>
#include "rssfeed.h"

class XmlReader: public QObject
{
    Q_OBJECT
public:
    XmlReader();
    void readXmlData(QNetworkReply *reply);
    void redirectReply(QNetworkReply *reply);
    QList<RssFeed> getArticleList();

private:
    int checkIfInList(QString title, QString desc, QString pubDate);
     QList<RssFeed> articleList;
};




#endif // XMLREADER_H
