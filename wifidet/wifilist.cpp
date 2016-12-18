#include "wifilist.h"

// Compares SSID a BSSID of WiFi networks
int isSameWifi(allWifiInfo_t le, wifiInfo_t ae){
  return !strcmp(le.id.SSID, ae.id.SSID) && !strcmp(le.id.BSSID, ae.id.BSSID);
}

// A function for the allWifiInto_t list sorting using the quicksort algorithm
int sortAWLf(allWifiInfo_t &a, allWifiInfo_t &b){
  if(a.i.signal > 0 || b.i.signal > 0) // 1. signal (more = better)
    return a.i.signal > b.i.signal;
  if(a.t.last != b.t.last)        // 2. time "last seen" (more recent = better)
    return a.t.last > b.t.last;
  if(strcmp(a.id.SSID,b.id.SSID)) // 3. SSID (alphabetical, more = worse)
    return strcmp(a.id.SSID,b.id.SSID) < 0;
  return strcmp(a.id.BSSID,b.id.BSSID) < 0; // 4. BSSID

}

// Sets the signal and the channel from the current state of the network
// and current time as the moment, when was the WiFi last seen
void updateWifiInfo(allWifiInfo_t &le, wifiInfo_t ae){
  le.i.channel = ae.i.channel;
  le.i.signal = ae.i.signal;
  le.t.last = QDateTime::currentDateTime();
}

// Overloaded previous function when the network is not currently available
void updateWifiInfo(allWifiInfo_t &le){
  le.i.channel = 0;
  le.i.signal = 0;
}

// Updates the list of all networks based on the list of currently available networks
int updateWifiList(QList<allWifiInfo_t> &l, QList<wifiInfo_t> &a){
  // Updates the recorded networks, that are currently available
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

  // Sorts the networks, since after the update their order might have changed
  qSort(l.begin(), l.end(), sortAWLf);

  // Inserts networks, that weren't previously recorded
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
