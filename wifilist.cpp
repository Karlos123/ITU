#include "wifilist.h"
//#ifdef QDEBUG
//#include <QDebug>
//#endif

// Porovna nazov a BSSID WiFi siete... Teoreticky by sa dalo porovnavat len BSSID, cim by nevznikali zaznamy sieti s rozlicnymi nazvami a rovnakym BSSID
int isSameWifi(allWifiInfo_t le, wifiInfo_t ae){
  //qDebug() << le.id.SSID << " " << ae.id.SSID << (le.id.SSID == ae.id.SSID) << "\t" << le.id.BSSID << " " << ae.id.BSSID << (le.id.BSSID == ae.id.BSSID);
  return !strcmp(le.id.SSID, ae.id.SSID) && !strcmp(le.id.BSSID, ae.id.BSSID);
}

// Funkcia pre usporiadanie listu typu allWifiInfo_t quicksortom (dalo by sa to prerobit na sablonu)
int sortAWLf(allWifiInfo_t &a, allWifiInfo_t &b){
  if(a.i.signal > 0 || b.i.signal > 0) // 1. signal (viac = lepsie)
    return a.i.signal > b.i.signal;
  if(a.t.last != b.t.last)        // 2. cas kedy boli naposledy videne (viac = lepsie)
    return a.t.last > b.t.last;
  if(strcmp(a.id.SSID,b.id.SSID)) // 3. SSID (abecedne, viac = horsie)
    return strcmp(a.id.SSID,b.id.SSID) < 0;
  return strcmp(a.id.BSSID,b.id.BSSID) < 0; // 4. BSSID

}

// Nastavi signal a kanal v aktualnom stave a aktualny cas ako moment, kedy bolo naposledy wifi videne
void updateWifiInfo(allWifiInfo_t &le, wifiInfo_t ae){
  le.i.channel = ae.i.channel;
  le.i.signal = ae.i.signal;
  le.t.last = QDateTime::currentDateTime();
}

// Bez ae sa nuluje kanal a signal, teda WiFi nie je v dosahu
void updateWifiInfo(allWifiInfo_t &le){
  le.i.channel = 0;
  le.i.signal = 0;
}

// Aktualizuje plny zoznam sieti podla zoznamu aktualne dostupnych sieti
int updateWifiList(QList<allWifiInfo_t> &l, QList<wifiInfo_t> &a){

  for(allWifiInfo_t &le : l){
    bool u = false;
    foreach(wifiInfo_t ae, a){
      if(isSameWifi(le, ae)){
        u = true;
        updateWifiInfo(le, ae);
        break;
      }
    }
    if(u)
      continue;
    else
      updateWifiInfo(le);
  }

  qSort(l.begin(), l.end(), sortAWLf);

  foreach(wifiInfo_t ae, a){
    bool u = false;
    for(allWifiInfo_t &le : l){
      if(isSameWifi(le, ae)){
        u = true;
        break;
      }
      else if(le.i.signal == 0)
        break;
    }
    if(u)
      continue;
    else{
      allWifiInfo_t n;
      n.id = ae.id;
      n.i = ae.i;
      n.t.first = n.t.last = QDateTime::currentDateTime();
      wifiListInsert(l, n);
    }
  }

  return 0;
}
