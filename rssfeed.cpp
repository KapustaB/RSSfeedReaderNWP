#include "rssfeed.h"
#include <QString>

RssFeed::RssFeed()
{

}
RssFeed::~RssFeed(){

}

RssFeed::RssFeed(QString title, QString description, QString pubDate){
    this->title = title;
    this->description = description;
    this->pubDate = pubDate;
}

void RssFeed::setTitle(QString title){
    this->title = title;
}

QString RssFeed::getTitle(){
    return this->title;
}

void RssFeed::setDescription(QString description){
    this->description = description;
}

QString RssFeed::getDescription(){
    return this->description;
}

void RssFeed::setPubDate(QString pubDate){
    this->pubDate = pubDate;
}

QString RssFeed::getPubDate(){
    return this->pubDate;
}
