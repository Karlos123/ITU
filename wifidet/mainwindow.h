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
    // Upper toolbar Menus and Buttons
    QMenu *networksMenu;
    QMenu *aboutMenu;
    QAction *importAct;
    QAction *clearAct;
    QAction *aboutAct;
    ~MainWindow();

private slots:
    // UI elements' reactions
    void on_btnStart_clicked();
    void on_btnPause_clicked();
    void on_btnSave_clicked();
    void on_speedSlider_sliderMoved(int position);

    // Updates the list of networks
    void getWifis(); 
    void updateList();

    // Prints info about the selected network
    void on_listWidget_itemSelectionChanged();

    // Toolbar buttons' handlers
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
