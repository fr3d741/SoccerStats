#ifndef DATAMANAGER_H
#define DATAMANAGER_H

class TableStruct;

#include <QString>
#include <QList>
#include <QVector>
#include <QMap>

#include <memory>

#include "Interfaces/Condition.h"

class DataManager
{
	 QMap<QString, QList<std::shared_ptr<TableStruct>>> _tables;
	 int                 _maxColumnNumber;
	 QList<std::shared_ptr<Condition>>	_rulesForTables;
public:
	 typedef QMap<QString, QList<std::shared_ptr<TableStruct>> > TeamTablesContainer;

	 DataManager();

	 void ParseHtml(QString team, QString content);

	 int GetMaxColumnNumber();

	 QString Serialize();

	 void Deserialize(QString content);

	 const TeamTablesContainer& GetTables();

	 void AddConditionForTables(std::shared_ptr<Condition> condition);

	 QList<std::shared_ptr<TableStruct>>& GetTables(QString team);
private:
	 void buildTables(QString team, QList<QVector<QStringList>>& tables);
	 void buildRows(std::shared_ptr<TableStruct> table, QVector<QStringList>& rows);
};

#endif // DATAMANAGER_H
