#ifndef LISTCHECKER_H
#define LISTCHECKER_H

#include <stdlib.h>
#include <qdebug.h>
#include <QListWidget>

class ListChecker
{
public:
    ListChecker();
    bool checkIfListItemExist(QString link, QListWidget* listWidget);
};

#endif // LISTCHECKER_H
