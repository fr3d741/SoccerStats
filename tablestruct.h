#ifndef TABLESTRUCT_H
#define TABLESTRUCT_H

#include <QXmlStreamWriter>
#include <QList>
#include <QString>

struct Row{
    QList<QString> cells;

    void Serialize(QXmlStreamWriter &stream);
};

class TableStruct
{
public:
    TableStruct();

    QList<Row> rows;

    void Serialize(QXmlStreamWriter &stream);
private:
};

#endif // TABLESTRUCT_H
