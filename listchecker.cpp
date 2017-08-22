#include "listchecker.h"

ListChecker::ListChecker()
{

}

bool ListChecker::checkIfListItemExist(QString link, QListWidget* listWidget){
    QList<QListWidgetItem*> itemList;
    itemList = listWidget->findItems(link, 0);
    if(!itemList.isEmpty()){
        qDebug() << "Link already exists!";
        return true;
    }
        return false;
}
