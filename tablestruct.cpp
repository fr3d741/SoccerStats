#include "tablestruct.h"

TableStruct::TableStruct(QXmlStreamReader &reader)
{
    while(!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            if (reader.name() == "table")
            {
                break;
            }
        }
        else
            reader.readNext();
    }
}
