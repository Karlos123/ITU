#ifndef PARSERS_H
#define PARSERS_H

#include <QProcess>
#include "wifilist.h"

// Parsovanie vystupu z nmcli
int getNetworks_nmcli(QProcess &p, QList<wifiInfo_t> &l);

// Parsovanie vystupu z iwlist
int getNetworks_iwlist(QProcess &p, QList<wifiInfo_t> &l);

#endif // PARSERS_H
