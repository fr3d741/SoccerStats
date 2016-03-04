#ifndef TABLESTRUCT_H
#define TABLESTRUCT_H

#include <QList>
#include <QString>

struct Row{
    QList<QString> cells;
};

class TableStruct
{
public:
    TableStruct();

    QList<Row> rows;
private:
};

#endif // TABLESTRUCT_H
