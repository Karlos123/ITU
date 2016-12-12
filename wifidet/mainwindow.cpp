#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnStart_clicked()
{
    ui->btnPause->setEnabled(true);
    ui->btnStart->setEnabled(false);
    ui->btnSave->setEnabled(false);
}

void MainWindow::on_btnPause_clicked()
{
    ui->btnPause->setEnabled(false);
    ui->btnSave->setEnabled(true);
    ui->btnStart->setEnabled(true);
}





