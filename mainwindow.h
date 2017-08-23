#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QNetworkReply>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
