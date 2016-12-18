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
#include <QMessageBox>

int interval = 15; // Time interval between detections
QTimer *timer;
//int detectCount = 1; // Count of detections

// Constructor of the windows, initializes necessary components
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Attempts to import the stored history with the default name ".wifidetHistory.xml"
    l = importData();
    ui->setupUi(this);

    // Toolbar initialization
    createActions();
    createMenus();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getWifis()));
    ui->widget->hide();
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Initializes the menus on the upper toolbar
void MainWindow::createMenus(){
  networksMenu = menuBar()->addMenu(tr("&Networks"));
  networksMenu->addAction(importAct);
  networksMenu->addAction(clearAct);

  aboutMenu = menuBar()->addMenu(tr("&About"));
  aboutMenu->addAction(aboutAct);
}

// Initializes buttons in the menus on the upper toolbar
void MainWindow::createActions(){
  importAct = new QAction(tr("&Import..."), this);
  importAct->setStatusTip(tr("Import a list of previously saved networks"));
  connect(importAct, &QAction::triggered, this, &MainWindow::importNetworks);

  clearAct = new QAction(tr("&Clear list"), this);
  clearAct->setStatusTip(tr("Remove all networks from the list"));
  connect(clearAct, &QAction::triggered, this, &MainWindow::clearNetworks);

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

// Handler of the "Import..." button click, shows a dialogue box to open the file with stored history
void MainWindow::importNetworks(){
  QMessageBox::information(this, tr("Import"),
          tr("Importing the list of networks removes\ncurrently recorded networks."));

  QString fileName = QFileDialog::getOpenFileName(NULL, tr("Import networks..."), ".wifidetHistory.xml", tr("XML Files (*.xml)"));

  if(fileName.isNull())
    return;
  l = importData(fileName);
  updateList();
  ui->textBrowser->hide();
}

// Removes all the networks from the list
void MainWindow::clearNetworks(){
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Clear networks", "Are you sure you want to\n clear the list of networks?",
                                QMessageBox::No|QMessageBox::Yes);
  if (reply == QMessageBox::Yes) {
    l.clear();
    ui->listWidget->clear();
    ui->textBrowser->hide();
  }
}

// Prints some basic information about the application
void MainWindow::about(){
      QMessageBox::about(this, tr("About..."),
              tr("<b>Brief info:</b><br>"
                 "WiFiDeT was created as a project to User Interface Programming subject at Brno University of Technology "
                 "(Faculty of Inforamtion Technologies). "
                 "Main purpose of WiFiDeT is to capture wifi networs around the current station it is running on."
                 "<br>"
                 "<b>Authors:</b><br>"
                 "Andrej Barna and Karel Jiranek"));
}

// Updates the list of networks in the UI
void MainWindow::updateList(){
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
    if(a.i.signal){
        i->setTextColor(Qt::black);
        QString s;
        s.setNum(a.i.signal);
        i->setText(i->text() + " @ " + s + "%");
    }
    else
      i->setTextColor(Qt::gray);
    ui->listWidget->addItem(i);
    if(i->toolTip() == ui->label_ssNa->text() && i->whatsThis() == ui->label_bsAd->text())
      ui->listWidget->setCurrentItem(i);
    foreach(allWifiInfo_t aa, l){
      if(!strcmp(a.id.SSID, aa.id.SSID) && strcmp(a.id.BSSID,aa.id.BSSID)){
        QListWidgetItem *ii = new QListWidgetItem;
        ii->setText("  (" + QString(aa.id.BSSID) + ")");
        ii->setToolTip(aa.id.SSID);
        ii->setWhatsThis(aa.id.BSSID);
        if(aa.i.signal){
          ii->setTextColor(Qt::black);
          QString s;
          s.setNum(aa.i.signal);
          ii->setText(ii->text() + " @ " + s + "%");
        }
        else
          ii->setTextColor(Qt::gray);
        ui->listWidget->addItem(ii);
        if(ii->toolTip() == ui->label_ssNa->text() && ii->whatsThis() == ui->label_bsAd->text())
          ui->listWidget->setCurrentItem(ii);
      }
    }
  }
}

// Detection of the networks, updating of the list of all networks and updating
// of the list in the UI
void MainWindow::getWifis(){
    QProcess p;
    QList <struct wifiInfo> a;
    std::string out; // Output message

    //ui->textBrowser->clear(); // Clearing of the output box

    // Printing of the output header
    /*ui->textBrowser->setAlignment(Qt::AlignCenter);
    out = "Detection number: " + std::to_string(detectCount) + "\n";
    ui->textBrowser->append(out.c_str());
    detectCount++; // Increase the count of detections
    ui->textBrowser->setAlignment(Qt::AlignLeft);*/

    // Detects wifi networks
    p.setProcessChannelMode(QProcess::MergedChannels);
    p.start("nmcli", QStringList() << "-f" << "SSID,BSSID,CHAN,SIGNAL" << "dev" << "wifi");
    p.waitForFinished();
    if(p.exitCode()){
      p.start("sudo", QStringList() << "iwlist" << "scan");
      p.waitForFinished();
      if(p.exitCode()){
        return; // Command couldn't have been executed, probably user doesn't have root privileges?
      }
      getNetworks_iwlist(p, a);
    }
    else{
      getNetworks_nmcli(p, a);
    }

    updateWifiList(l, a);
    updateList();

    // Print of all available networks in the output box
    /*foreach(allWifiInfo_t net, l){
        out = "Name: " + std::string(net.id.SSID) + "\n" + "BSSID: " +net.id.BSSID + "\n" +
                "Channel: " +std::to_string((int) net.i.channel) + "\n" +  "Siganl strength: " +std::to_string((int) net.i.signal) + "\n";
        ui->textBrowser->append(out.c_str());
    }


    // Moves the cursor to the beginning and shows the beginning of the text in the output box
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);*/
}

// Start button handler
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

// Pause button handler
void MainWindow::on_btnPause_clicked()
{
    timer->stop(); // Stops the timer
    //detectCount = 1;
    ui->btnPause->setEnabled(false);
    ui->btnSave->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->speedSlider->setEnabled(true);
}

// Save button handler, opens up the dialog box to save the current list
// Default filename is the name of autoloaded file
void MainWindow::on_btnSave_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save History..."), ".wifidetHistory.xml", tr("XML Files (*.xml)")); // ;;All Files(*.*)

  if(fileName.isNull()){
    return;
  }
  else{
    if(!fileName.endsWith(".xml"))
      fileName += ".xml";
    if(exportData(l,fileName)){
        on_btnSave_clicked();
    }
  }
}

// Change of network detection interval handling
void MainWindow::on_speedSlider_sliderMoved(int position)
{
    interval = position;
    ui->lblInterval->setText(QString::fromStdString(std::to_string(interval) + " s"));
}

// Updates the information about the selected network
void MainWindow::on_listWidget_itemSelectionChanged(){
  foreach(allWifiInfo_t e, l){
    if(ui->listWidget->currentItem()->toolTip() == e.id.SSID && ui->listWidget->currentItem()->whatsThis() == e.id.BSSID){
      ui->label_ssNa->setText(e.id.SSID);
      ui->label_bsAd->setText(e.id.BSSID);

      ui->label_sgIcoHig->hide();
      ui->label_sgIcoMed->hide();
      ui->label_sgIcoLow->hide();
      QString s;
      if(e.i.signal){
        s.setNum(e.i.signal);
        ui->label_sgNo->setText(s + "%");
        if(e.i.signal >= 70)
          ui->label_sgIcoHig->show();
        else if(e.i.signal >= 40)
          ui->label_sgIcoMed->show();
        else
          ui->label_sgIcoLow->show();
      }
      else
        ui->label_sgNo->setText("N/A");

      if(e.i.channel){
        s.setNum(e.i.channel);
        ui->label_chNo->setText(s);
      }
      else ui->label_chNo->setText("N/A");

      ui->label_fsT->setText(e.t.first.toString("d. MMM yyyy, HH:mm"));
      ui->label_lsT->setText(e.t.last.toString("d. MMM yyyy, HH:mm"));

      ui->widget->show();
      break;
    }
  }
}
