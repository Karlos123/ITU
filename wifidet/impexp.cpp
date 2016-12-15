#include "impexp.h"

QList<allWifiInfo_t>* importData(QString filename){
  QList<allWifiInfo_t> *l = new QList<allWifiInfo_t>();

  QXmlStreamReader x(filename);
  while(!x.atEnd()){
    x.readNext();

  }
  if(x.hasError()){
        //... // do error handling
  }
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
