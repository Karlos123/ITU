#ifndef PARSERS_H
#define PARSERS_H

#include <QProcess>
#include "wifilist.h"

// Output parsing from the nmcli command
int getNetworks_nmcli(QProcess &p, QList<wifiInfo_t> &l);

// Output parsing from the iwlist command
int getNetworks_iwlist(QProcess &p, QList<wifiInfo_t> &l);

#endif // PARSERS_H
