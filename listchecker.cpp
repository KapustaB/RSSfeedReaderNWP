#include "listchecker.h"
#include <QTreeWidget>
ListChecker::ListChecker()
{

}

bool ListChecker::checkIfListItemExist(QString link, QTreeWidget* treeWidget){
    QList<QTreeWidgetItem*> itemList;
    itemList = treeWidget->findItems(link, Qt::MatchExactly, 0);
    if(!itemList.isEmpty()) return true;
    return false;
}
