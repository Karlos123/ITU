#include "impexp.h"

// Reads the list of all networks from an XML file
QList<allWifiInfo_t> importData(QString filename){
  QList<allWifiInfo_t> l;
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly))
    return l; // Couldn't open the specified file, empty list is returned

  QXmlStreamReader x(&file);
  allWifiInfo_t current;

  while(!x.atEnd()){
    x.readNext();
    if(x.isStartDocument() || x.name() == "wifiRecords")
      continue;
    else{
      if(x.isStartElement()){
        // SSID a BSSID reading
        strcpy(current.id.SSID, x.name().toString().toStdString().data());
        if(x.attributes().hasAttribute("BSSID")){
          strcpy(current.id.BSSID, x.attributes().value("BSSID").toString().toStdString().data());
        }
        else{
          // BSSID missing
          strcpy(current.id.BSSID, "< U N K N O W N >");
          //x.error();
        }

        // Reading of the remaining informations about the network, in our case these are stored time values
        for(int i = 0; i < 2; i++){
          x.readNextStartElement();
          if(x.name() == "firstSeen")
            current.t.first = QDateTime::fromString(x.readElementText(), Qt::ISODate).toLocalTime();
          else if(x.name() == "lastSeen")
            current.t.last = QDateTime::fromString(x.readElementText(), Qt::ISODate).toLocalTime();
          x.readNext();
        }

        x.readNext(); // Reads the tag closing the element
        current.i.signal = current.i.channel = 0;
        wifiListInsert(l, current);
      }
      else{
        if(x.name() == "" && x.readElementText() == "")
          continue;
        x.error();
      }
    }
  }
  if(x.hasError()){
    l.clear();
  }

  // Sorts the imported list of networks
  qSort(l.begin(), l.end(), sortAWLf);
  return l;
}


// Saves the list of all networks into an XML file
int exportData(QList<allWifiInfo_t> &l, QString filename){
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly))
    return 1;
  QXmlStreamWriter x(&file);
  x.setAutoFormatting(true);
  x.writeStartDocument();
  x.writeStartElement("wifiRecords");

  // Prints all WiFi networks in the list
  foreach(allWifiInfo_t le, l){
    x.writeStartElement(le.id.SSID);
    x.writeAttribute("BSSID", le.id.BSSID);
    x.writeTextElement("firstSeen",le.t.first.toUTC().toString(Qt::ISODate));
    x.writeTextElement("lastSeen",le.t.last.toUTC().toString(Qt::ISODate));
    x.writeEndElement();
  }

  // Closes the "wifiRecords" element and ends the document
  x.writeEndElement();
  x.writeEndDocument();
  return 0;
}
