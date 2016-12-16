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

private:
    Ui::MainWindow *ui;
    QList <struct allWifiInfo> l;
};




#endif // MAINWINDOW_H
