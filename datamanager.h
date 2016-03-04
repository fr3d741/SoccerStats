#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QList>
#include <QVector>

#include "tablestruct.h"

class DataManager
{
    QList<TableStruct*> _tables;
    int                 _maxColumnNumber;
public:
    DataManager();

    void ParseHtml(QString content);

    int GetMaxColumnNumber();

    QList<TableStruct*>& GetTables();
private:
    void buildTables(QList<QVector<QStringList>>& tables);
    void buildRows(TableStruct* table, QVector<QStringList>& rows);
};

#endif // DATAMANAGER_H
