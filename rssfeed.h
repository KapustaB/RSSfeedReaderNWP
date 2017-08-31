#ifndef RSSFEED_H
#define RSSFEED_H
#include <QString>


class RssFeed
{
public:
    RssFeed();
    ~RssFeed();
    RssFeed(QString title, QString description, QString pubDate);

    void setTitle(QString title);
    void setDescription(QString description);
    void setPubDate(QString pubDate);

    QString getTitle();
    QString getDescription();
    QString getPubDate();

private:
    QString title, description, pubDate;
};

#endif // RSSFEED_H
