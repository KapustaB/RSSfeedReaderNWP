#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QNetworkReply>
#include <QPrinter>
#include <QTreeWidgetItem>
#include "rssfeed.h"
#include "xmlreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addLinkBtn_clicked();

    void on_removeLinkBtn_clicked();

    void on_removeLinkBtn_clicked(bool checked);

    void requestReceived(QNetworkReply *reply);

    void on_saveFeedBtn_clicked();

    void on_printFeedBtn_clicked();

    void on_linksTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_linksTreeWidget_clicked(const QModelIndex &index);

    void print(QPrinter* printer);

private:
    Ui::MainWindow *ui;
    QList<RssFeed> articleList;
    QMap<int, QList<RssFeed>> rssMapa;
};

#endif // MAINWINDOW_H
