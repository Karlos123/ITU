#include "mainwindow.h"
#include "parsers.h"
#include <QApplication>
#include "ui_mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("QMainWindow {background: '#DEDEFF';}");
    w.show();

    return a.exec();
}


