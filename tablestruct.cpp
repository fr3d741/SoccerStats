#include <QXmlStreamWriter>
#include "tablestruct.h"

TableStruct::TableStruct()
{
}

int TableStruct::maxColumnNumber()
{
    int max = 0;
    foreach (auto var, rows) {
        max = std::max(var.maxColumnNumber(), max);
    }
    return max;
}

void TableStruct::Serialize(QXmlStreamWriter& stream)
{
    stream.writeStartElement("Table");
    stream.writeAttribute("rowCount", QString("%1").arg(rows.size()));
    foreach (auto var, rows) {
       var.Serialize(stream);
    }
    stream.writeEndElement();
}

void TableStruct::Deserialize(QXmlStreamReader &stream)
{
    QXmlStreamReader::TokenType type = stream.tokenType();
    QString name = stream.name().toString();
    if (type == QXmlStreamReader::StartElement && name == "Table")
    {
        auto attributes = stream.attributes();
        bool ok = false;
        int count = attributes.value("rowCount").toInt(&ok);
        while(count)
        {
            if (stream.readNext() == QXmlStreamReader::StartElement)
            {
                Row r;
                r.Deserialize(stream);
                rows.push_back(r);
                count--;
            }
        }
    }
}


int Row::maxColumnNumber()
{
    return cells.size();
}

void Row::Serialize(QXmlStreamWriter& stream)
{
    stream.writeStartElement("Row");
    stream.writeAttribute("count", QString("%1").arg(cells.size()));
    foreach (auto var, cells) {
       stream.writeTextElement("Cell", var);
    }
    stream.writeEndElement();
}

void Row::Deserialize(QXmlStreamReader &stream)
{
    QXmlStreamReader::TokenType type = stream.tokenType();
    QString name = stream.name().toString();
    if (type == QXmlStreamReader::StartElement && name == "Row")
    {
        auto attributes = stream.attributes();
        bool ok = false;
        int count = attributes.value("count").toInt(&ok);
        while(count)
        {
            QXmlStreamReader::TokenType subType = stream.readNext();
            if (subType == QXmlStreamReader::StartElement && stream.name() == "Cell")
            {
                cells.push_back(stream.readElementText());
                count--;
            }
        }
    }
}
