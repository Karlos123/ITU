#ifndef IMPEXP_H
#define IMPEXP_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include "wifilist.h"

QList<allWifiInfo_t> importData(QString filename = ".wifidetHistory.xml");

int exportData(QList<allWifiInfo_t> &l, QString filename = ".wifidetHistory.xml");

#endif // IMPEXP_H
