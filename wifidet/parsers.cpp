#include "parsers.h"
//#include <iostream>
//#include <QCoreApplication>

// Parsovanie vystupu z nmcli -f SSID,BSSID,CHAN,SIGNAL dev wifi
int getNetworks_nmcli(QProcess &p, QList<wifiInfo_t> &l){
  l.clear();
  QString o = p.readAllStandardOutput();
  QStringList nets = o.split('\n', QString::SkipEmptyParts);
  nets.removeFirst();
  foreach(QString row, nets){
    QString buffer = row.section(' ', 0, 0, QString::SectionSkipEmpty);
    //if(buffer == "SSID") // Prvy riadok, preskocime ho
    //  continue;
    wifiInfo_t w;
    strcpy(w.SSID, buffer.toStdString().data());
    buffer = row.section(' ', 1, 1, QString::SectionSkipEmpty);
    strcpy(w.BSSID, buffer.toStdString().data());
    buffer = row.section(' ', 2, 2, QString::SectionSkipEmpty);
    w.channel = buffer.toUInt();
    buffer = row.section(' ', 3, 3, QString::SectionSkipEmpty);
    w.signal = buffer.toUInt();

    if(l.empty())
      l.append(w);
    else{
      if(w.signal < l.last().signal)
        l.append(w);
      else if(w.signal > l.first().signal)
        l.prepend(w);
      else
        for(int i = 0; i < l.size()-1; i++)
          if(l[i].signal > w.signal && l[i+1].signal < w.signal){
            l.insert(i+1, w);
            break;
          }
    }
  }
  return l.size();
}

// Parsovanie vystupu zo sudo iwlist scan
int getNetworks_iwlist(QProcess &p, QList<wifiInfo_t> &l){
  l.clear();
  QString o = p.readAllStandardOutput();
  QStringList nets = o.split("Cell ", QString::SkipEmptyParts);
  nets.removeFirst();
  foreach(QString row, nets){
    wifiInfo_t w;
    QString buffer = row.mid(row.indexOf("Address: ")+9, 17);
    strcpy(w.BSSID, buffer.toStdString().data());
    buffer = row.mid(row.indexOf("Channel:")+8, 2);
    w.channel = buffer.simplified().toUInt();
    buffer = row.mid(row.indexOf("Signal level=")+13, 3);
    int signaldBm = buffer.toInt();
    if(signaldBm > -45)
      w.signal = 100;
    else if(signaldBm < -95)
      w.signal = 0;
    else
      w.signal = (signaldBm+95)*2;
    // vysledok v porovnani s nmcli je +-2%

    int essidpos = row.indexOf("ESSID:\"")+7;
    buffer = row.mid(essidpos, row.indexOf('"', essidpos)-essidpos);
    strcpy(w.SSID, buffer.toStdString().data());

    if(l.empty())
      l.append(w);
    else{
      if(w.signal < l.last().signal)
        l.append(w);
      else if(w.signal > l.first().signal)
        l.prepend(w);
      else
        for(int i = 0; i < l.size()-1; i++)
          if(l[i].signal > w.signal && l[i+1].signal < w.signal){
            l.insert(i+1, w);
            break;
          }
    }
  }
  return l.size();
}

/*
// Ziska a vypise obsah zoznamu wifi sieti, skusa najprv nmcli a ak to nefunguje tak skusi iwlist scan
int main(int argc, char *argv[]){
  QCoreApplication a(argc, argv);

  QProcess p;
  QList <struct wifiInfo> l;

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

  foreach(wifiInfo_t net, l){
    std::cout << net.SSID << "\t" << net.BSSID << "\t" << (int) net.channel << "\t" << (int) net.signal << std::endl;
  }

  return a.exec();
}
*/
