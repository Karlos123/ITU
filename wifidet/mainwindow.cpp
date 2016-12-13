#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parsers.h"
#include <QApplication>
#include <string>
#include <iostream>
#include <QToolButton>
#include <QMenuBar>
#include <unistd.h>
#include <QTimer>


int interval = 15; // Casovy usek mezi jednotlivymi detekcemi wifi siti
QTimer *timer;
int detectCount = 1; // Pocet detekovani wifi siti

void getWifis();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getWifis()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Detekce a vypis wifi siti */
void MainWindow::getWifis(){
    QProcess p;
    QList <struct wifiInfo> l;
    std::string out; // Zprava na vypsani

    ui->textBrowser->clear(); // Vycisteni okna

    // Vypsani hlavicky vypisu
    ui->textBrowser->setAlignment(Qt::AlignCenter);
    out = "Detection number: " + std::to_string(detectCount) + "\n";
    ui->textBrowser->append(out.c_str());
    detectCount++; // Navyseni poctu detekci
    ui->textBrowser->setAlignment(Qt::AlignLeft);

    // Detekce wifi
    p.setProcessChannelMode(QProcess::MergedChannels);
    p.start("nmcli", QStringList() << "-f" << "SSID,BSSID,CHAN,SIGNAL" << "dev" << "wifi");
    p.waitForFinished();
    if(p.exitCode()){
      p.start("sudo", QStringList() << "iwlist" << "scan");
      p.waitForFinished();
      if(p.exitCode()){
        ; // nema superuser opravnenia, nezadal heslo, whatever, moze sa ist .........
      }
      getNetworks_iwlist(p, l);
    }
    else{
      getNetworks_nmcli(p, l);
    }

    // Vypis dostupnych wifi
    foreach(wifiInfo_t net, l){
        out = "Name: " + std::string(net.SSID) + "\n" + "BSSID: " +net.BSSID + "\n" +
                "Channel: " +std::to_string((int) net.channel) + "\n" +  "Siganl strength: " +std::to_string((int) net.signal) + "\n";
        ui->textBrowser->append(out.c_str());
    }


    // Premisteni kurzoru, pohledu na zacatek
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);



}

/* Tlacitko START */
void MainWindow::on_btnStart_clicked()
{

    ui->btnPause->setEnabled(true);
    ui->btnStart->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->speedSlider->setEnabled(false);
    ui->textBrowser->setAlignment(Qt::AlignLeft);
    getWifis(); // Ziskani wifi siti
    timer->start(interval*1000); // Odpaleni casovace


}

/* Tlacitko PAUSE */
void MainWindow::on_btnPause_clicked()
{
    timer->stop(); // Zastaveni casovace
    detectCount = 1;
    ui->btnPause->setEnabled(false);
    ui->btnSave->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->speedSlider->setEnabled(true);
}






/* Zmena velikosti intervalu detekce wifi siti */
void MainWindow::on_speedSlider_sliderMoved(int position)
{
    interval = position;
    ui->lblInterval->setText(QString::fromStdString(std::to_string(interval) + " s"));

}
