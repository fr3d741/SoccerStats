#ifndef DATAMANAGER_H
#define DATAMANAGER_H

class TableStruct;

#include <QString>
#include <QList>
#include <QVector>
#include <QMap>

class DataManager
{
    QMap<QString, QList<TableStruct*> > _tables;
    int                 _maxColumnNumber;
public:
    typedef QMap<QString, QList<TableStruct*> > TeamTablesContainer;

    DataManager();

    void ParseHtml(QString team, QString content);

    int GetMaxColumnNumber();

    QString Serialize();

    void Deserialize(QString content);

    const TeamTablesContainer& GetTables();
    QList<TableStruct*>& GetTables(QString team);
private:
    void buildTables(QString team, QList<QVector<QStringList>>& tables);
    void buildRows(TableStruct* table, QVector<QStringList>& rows);
};

#endif // DATAMANAGER_H
