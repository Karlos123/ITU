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



    void on_speedSlider_sliderMoved(int position);

    void getWifis();

private:
    Ui::MainWindow *ui;
    QList <struct allWifiInfo> l;
};




#endif // MAINWINDOW_H
