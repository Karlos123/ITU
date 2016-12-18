#ifndef IMPEXP_H
#define IMPEXP_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include "wifilist.h"

// Reads stored WiFi networks history from an XML file and returns it as a list of networks
QList<allWifiInfo_t> importData(QString filename = ".wifidetHistory.xml");

// Stores WiFi networks history in an XML file
int exportData(QList<allWifiInfo_t> &l, QString filename = ".wifidetHistory.xml");

#endif // IMPEXP_H
