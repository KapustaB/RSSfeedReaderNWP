#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QNetworkReply>
#include <QPrinter>

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

    void on_linkListWidget_itemClicked(QListWidgetItem *item);

    void on_removeLinkBtn_clicked();

    void on_removeLinkBtn_clicked(bool checked);

    void requestReceived(QNetworkReply *reply);

    void on_linkListWidget_doubleClicked(const QModelIndex &index);

    void updateText(const QString & newText);

    void on_saveFeedBtn_clicked();

    void on_printFeedBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
