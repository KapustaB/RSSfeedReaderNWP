#ifndef LISTCHECKER_H
#define LISTCHECKER_H

#include <stdlib.h>
#include <qdebug.h>
#include <QTreeWidget>

class ListChecker
{
public:
    ListChecker();
    bool checkIfListItemExist(QString link, QTreeWidget* treeWidget);
};

#endif // LISTCHECKER_H
