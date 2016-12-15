#ifndef IMPEXP_H
#define IMPEXP_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include "wifilist.h"

QList<allWifiInfo_t>* importData(QString filename = ".wifidetHistory");

int exportData(QList<allWifiInfo_t> &l, QString filename = ".wifidetHistory");

#endif // IMPEXP_H
