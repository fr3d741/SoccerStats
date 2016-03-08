#include <QXmlStreamWriter>
#include "tablestruct.h"

TableStruct::TableStruct()
{
}

void TableStruct::Serialize(QXmlStreamWriter& stream)
{
    stream.writeStartElement("Table");
    foreach (auto var, rows) {
       var.Serialize(stream);
    }
    stream.writeEndElement();
}


void Row::Serialize(QXmlStreamWriter& stream)
{
    stream.writeStartElement("Row");
    foreach (auto var, cells) {
       stream.writeTextElement("Cell", var);
    }
    stream.writeEndElement();
}
