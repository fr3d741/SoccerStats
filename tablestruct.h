#ifndef TABLESTRUCT_H
#define TABLESTRUCT_H

#include <QList>
#include <QXmlStreamReader>

struct Cell{
    QString value;
};

struct Row{
    QList<Cell> cells;
};

class TableStruct
{
public:
    TableStruct(QXmlStreamReader& reader);

    QList<Row> rows;
private:
    void ExtractRow(QXmlStreamReader& reader);
    void ExtractCell(QXmlStreamReader& reader);
};

#endif // TABLESTRUCT_H
