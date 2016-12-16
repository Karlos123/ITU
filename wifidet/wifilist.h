#ifndef WIFILIST_H
#define WIFILIST_H

#include <QDateTime>
#include <QString>
#include <QList>
#include <string.h>

typedef struct {
  char SSID[33];  // Nazov WiFi
  char BSSID[18]; // MAC -- nema zmysel ich prekladat do normalnej 6bajtovej podoby, aj tak to len vypiseme
} ids_t; // IDs

typedef struct {
  QDateTime first;
  QDateTime last;
} tseen_t; // Time Seen

typedef struct {
  char channel;   // Cislo kanalu
  char signal;    // Sila signalu
  //bool active;
} awi_t; // Available WiFi Info

typedef struct wifiInfo {
  ids_t id;
  awi_t i;
} wifiInfo_t;

typedef struct allWifiInfo{
  ids_t id;
  tseen_t t;
  awi_t i;
} allWifiInfo_t;

typedef struct {
  ids_t id;
  tseen_t t;
} exportData_t;

//template<typename T>int wifiListInsert(QList<T&> &l, T& w);
template<typename T>
int wifiListInsert(QList<T> &l, T w){
  if(l.empty() || w.i.signal <= l.last().i.signal){
    l.append(w);
    return l.size()-1;
  }
  else if(w.i.signal >= l.first().i.signal)
    l.prepend(w);
  else
    for(int i = 0; i < l.size()-1; i++)
      if(l[i].i.signal > w.i.signal && l[i+1].i.signal < w.i.signal){
        l.insert(i+1, w);
        return i+1;
      }
  return 0;
}

int sortAWLf(allWifiInfo_t &a, allWifiInfo_t &b);
int updateWifiList(QList<allWifiInfo_t> &l, QList<wifiInfo_t> &a);

#endif // WIFILIST_H
