#ifndef XMLREADER_H
#define XMLREADER_H
#include <QNetworkReply>
#include <QMainWindow>

class XmlReader: public QObject
{
    Q_OBJECT
public:
    XmlReader();
    void readXmlData(QNetworkReply *reply);
    void redirectReply(QNetworkReply *reply);

signals:
    void sendText(const QString & newText);
};




#endif // XMLREADER_H
