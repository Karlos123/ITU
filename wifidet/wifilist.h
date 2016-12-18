#ifndef WIFILIST_H
#define WIFILIST_H

#include <QDateTime>
#include <QString>
#include <QList>
#include <string.h>

typedef struct {
  char SSID[33];  // WiFi name
  char BSSID[18]; // WiFi hotspot MAC address
} ids_t; // IDs

typedef struct {
  QDateTime first;
  QDateTime last;
} tseen_t; // Time Seen

typedef struct {
  char channel;   // Channel number
  char signal;    // Signal strength in %
  //bool active; -- other possible informations about the network
} awi_t; // Available WiFi Info

// Informations about currently available networks
typedef struct wifiInfo {
  ids_t id;
  awi_t i;
} wifiInfo_t;

// All information about the networks
typedef struct allWifiInfo{
  ids_t id;
  tseen_t t;
  awi_t i;
} allWifiInfo_t;


// Inserts a list element into the list
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

// Function used to sort the list of all networks
int sortAWLf(allWifiInfo_t &a, allWifiInfo_t &b);

// Updates the list of all networks depending on the list of currently available networks
int updateWifiList(QList<allWifiInfo_t> &l, QList<wifiInfo_t> &a);

#endif // WIFILIST_H
