#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parsers.h"
#include "wifilist.h"
#include "impexp.h"
#include <QApplication>
#include <string>
#include <iostream>
#include <QToolButton>
#include <QMenuBar>
#include <unistd.h>
#include <QTimer>
#include <QFileDialog>

int interval = 15; // Casovy usek mezi jednotlivymi detekcemi wifi siti
QTimer *timer;
int detectCount = 1; // Pocet detekovani wifi siti

void getWifis();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    l = importData();
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getWifis()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizeList(){
  ui->listWidget->clear();
  QStringList p;
  foreach(allWifiInfo_t a, l){
    if(p.contains(a.id.SSID))
      continue;
    p.append(a.id.SSID);
    //ui->listWidget->addItem(QString() + "\n  (" + QString(a.id.BSSID) + ")");
    QListWidgetItem *i = new QListWidgetItem;
    i->setText(QString(a.id.SSID) + "\n  (" + QString(a.id.BSSID) + ")");
    i->setToolTip(a.id.SSID);
    i->setWhatsThis(a.id.BSSID);
    ui->listWidget->addItem(i);
    foreach(allWifiInfo_t aa, l){
      if(!strcmp(a.id.SSID, aa.id.SSID) && strcmp(a.id.BSSID,aa.id.BSSID)){
        QListWidgetItem *ii = new QListWidgetItem;
        ii->setText("  (" + QString(aa.id.BSSID) + ")");
        ii->setToolTip(aa.id.SSID);
        ii->setWhatsThis(aa.id.BSSID);
        ui->listWidget->addItem(ii);
      }
    }
  }
}

/* Detekce a vypis wifi siti */
void MainWindow::getWifis(){
    QProcess p;
    QList <struct wifiInfo> a;
    std::string out; // Zprava na vypsani

    // \/ vlozit do konstruktoru?
    //QList<allWifiInfo_t> *lul = importData();
    //delete lul;
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
        return; // nema superuser opravnenia, nezadal heslo, whatever, moze sa ist .........
      }
      getNetworks_iwlist(p, a);
    }
    else{
      getNetworks_nmcli(p, a);
    }

    updateWifiList(l, a);
    actualizeList();
    //ui->listWidget->repaint();
    //exportData(l);

    // Vypis dostupnych wifi
    /*foreach(allWifiInfo_t net, l){
        out = "Name: " + std::string(net.id.SSID) + "\n" + "BSSID: " +net.id.BSSID + "\n" +
                "Channel: " +std::to_string((int) net.i.channel) + "\n" +  "Siganl strength: " +std::to_string((int) net.i.signal) + "\n";
        ui->textBrowser->append(out.c_str());
    }*/


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
    ui->textBrowser->hide();

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


void MainWindow::on_btnSave_clicked()
{

  QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save History..."), ".wifidetHistory.xml", tr("XML Files (*.xml)")); // ;;All Files(*.*)

  // Aktualne to dookola ponuka ulozenie hry kym sa ju nepodari ulozit, alebo uzivatel neda Cancel
  if(fileName.isNull()){
    return;
  }
  else{
    if(!fileName.endsWith(".xml"))
      fileName += ".xml";
    if(exportData(l,fileName)){ // Nedalo sa ulozit hru or sth
        on_btnSave_clicked();
    }
  }
}

void MainWindow::on_actionOptions_triggered(){}

void MainWindow::on_btnSave_pressed(){}


/* Zmena velikosti intervalu detekce wifi siti */
void MainWindow::on_speedSlider_sliderMoved(int position)
{
    interval = position;
    ui->lblInterval->setText(QString::fromStdString(std::to_string(interval) + " s"));

}

void MainWindow::on_listView_viewportEntered(){}
void MainWindow::on_listWidget_activated(const QModelIndex &index){}

