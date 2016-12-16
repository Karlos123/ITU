#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QtGui>
#include "wifilist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMenu *networksMenu;
    QMenu *aboutMenu;
    QAction *importAct;
    QAction *clearAct;
    QAction *aboutAct;
    ~MainWindow();

private slots:
    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_actionOptions_triggered();

    void actualizeList();

    void on_speedSlider_sliderMoved(int position);

    void getWifis();

    void on_btnSave_pressed();

    void on_btnSave_clicked();

    void on_listView_viewportEntered();

    void on_listWidget_activated(const QModelIndex &index);

    void on_listWidget_itemSelectionChanged();

    void importNetworks();
    void clearNetworks();
    void about();

private:
    Ui::MainWindow *ui;
    QList <struct allWifiInfo> l;
    void createActions();
    void createMenus();
};




#endif // MAINWINDOW_H
