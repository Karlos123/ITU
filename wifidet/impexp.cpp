#include "impexp.h"
#include <QDebug>

QList<allWifiInfo_t>* importData(QString filename){
  QList<allWifiInfo_t> *l = new QList<allWifiInfo_t>();
  QFile file(filename);
  file.open(QIODevice::ReadOnly);
  QXmlStreamReader x(&file);
  allWifiInfo_t current;
  while(!x.atEnd()){
    x.readNext();
    if(x.isStartDocument() || x.name() == "wifiRecords")
      continue;
    else{
      if(x.isStartElement()){
        // Citanie SSID a BSSID
        strcpy(current.id.SSID, x.name().toString().toStdString().data());
        if(x.attributes().hasAttribute("BSSID")){
          strcpy(current.id.BSSID, x.attributes().value("BSSID").toString().toStdString().data());
        }
        else{
          // Mame problem, chyba BSSID
          strcpy(current.id.BSSID, "< U N K N O W N >");
          //x.error();
        }

        // Citanie dalsich ulozenych informacii, konkretne u nas sa jedna o ulozene casy
        for(int i = 0; i < 2; i++){
          x.readNextStartElement();
          if(x.name() == "firstSeen")
            current.t.first = QDateTime::fromString(x.readElementText(), Qt::ISODate);
          else if(x.name() == "lastSeen")
            current.t.last = QDateTime::fromString(x.readElementText(), Qt::ISODate);
          x.readNext();
        }

        x.readNext(); // uzatvorenie
        wifiListInsert(*l, current);
      }
      else{
        if(x.name() == "" && x.readElementText() == "")
          continue;

        //qDebug() << x.name();
        //qDebug() << x.readElementText();
        x.error();
      }
    }
  }
  if(x.hasError()){
    delete l;
    return nullptr;
  }
  /*
  foreach(allWifiInfo_t a, *l){
    qDebug() << a.id.SSID << " (" << a.id.BSSID << ")";
    qDebug() << a.t.first << " " << a.t.last;
  }
  */
  return l;
}

int exportData(QList<allWifiInfo_t> &l, QString filename){
  QFile file(filename);
  file.open(QIODevice::WriteOnly);
  QXmlStreamWriter x(&file);
  x.setAutoFormatting(true);
  x.writeStartDocument();
  x.writeStartElement("wifiRecords");

  // Pisanie
  foreach(allWifiInfo_t le, l){
    x.writeStartElement(le.id.SSID);
    x.writeAttribute("BSSID", le.id.BSSID);
    x.writeTextElement("firstSeen",le.t.first.toUTC().toString(Qt::ISODate));
    x.writeTextElement("lastSeen",le.t.last.toUTC().toString(Qt::ISODate));
    x.writeEndElement();
  }

  x.writeEndElement();
  x.writeEndDocument();
  return 0;
}
