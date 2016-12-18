#include "parsers.h"

// Output parsing from the "nmcli -f SSID,BSSID,CHAN,SIGNAL dev wifi" command
int getNetworks_nmcli(QProcess &p, QList<wifiInfo_t> &l){
  l.clear();
  QString o = p.readAllStandardOutput();
  QStringList nets = o.split('\n', QString::SkipEmptyParts);
  if(!nets.size())
    return 0;
  nets.removeFirst();
  foreach(QString row, nets){
    QString buffer = row.section(' ', 0, -4, QString::SectionSkipEmpty);
    wifiInfo_t w;
    strcpy(w.id.SSID, buffer.toStdString().data());
    buffer = row.section(' ', -3, -3, QString::SectionSkipEmpty);
    strcpy(w.id.BSSID, buffer.toStdString().data());
    buffer = row.section(' ', -2, -2, QString::SectionSkipEmpty);
    w.i.channel = buffer.toUInt();
    buffer = row.section(' ', -1, -1, QString::SectionSkipEmpty);
    w.i.signal = buffer.toUInt();

    wifiListInsert(l, w);
  }
  return l.size();
}

// Output parsing from the "sudo iwlist scan" command
int getNetworks_iwlist(QProcess &p, QList<wifiInfo_t> &l){
  l.clear();
  QString o = p.readAllStandardOutput();
  QStringList nets = o.split("Cell ", QString::SkipEmptyParts);
  if(!nets.size())
    return 0;
  nets.removeFirst();
  foreach(QString row, nets){
    wifiInfo_t w;
    QString buffer = row.mid(row.indexOf("Address: ")+9, 17);
    strcpy(w.id.BSSID, buffer.toStdString().data());
    buffer = row.mid(row.indexOf("Channel:")+8, 3);
    if(buffer[2] == ')')
      buffer[2] = 0;
    w.i.channel = buffer.simplified().toUInt();
    buffer = row.mid(row.indexOf("Signal level=")+13, 3);
    int signaldBm = buffer.toInt();
    if(signaldBm > -45)
      w.i.signal = 100;
    else if(signaldBm < -95)
      w.i.signal = 0;
    else
      w.i.signal = (signaldBm+95)*2;

    int essidpos = row.indexOf("ESSID:\"")+7;
    buffer = row.mid(essidpos, row.indexOf('"', essidpos)-essidpos);
    strcpy(w.id.SSID, buffer.toStdString().data());

    wifiListInsert(l, w);
  }
  return l.size();
}
