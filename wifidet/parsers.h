#ifndef PARSERS_H
#define PARSERS_H

#include <QProcess>

typedef struct wifiInfo {
  char SSID[33];  // Nazov WiFi
  char BSSID[18]; // MAC -- nema zmysel ich prekladat do normalnej 6bajtovej podoby, aj tak to len vypiseme
  char channel;   // Cislo kanalu
  char signal;    // Sila signalu
  //bool active;
} wifiInfo_t;

// Parsovanie vystupu z nmcli
int getNetworks_nmcli(QProcess &p, QList<wifiInfo_t> &l);

// Parsovanie vystupu z iwlist
int getNetworks_iwlist(QProcess &p, QList<wifiInfo_t> &l);

#endif // PARSERS_H
