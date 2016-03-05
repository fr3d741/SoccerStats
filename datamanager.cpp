#include "datamanager.h"

#include "htmlparser.h"
#include "tablestruct.h"

DataManager::DataManager()
    :_maxColumnNumber(0)
{
}

void DataManager::ParseHtml(QString team, QString content)
{
    HtmlParser parser;
    QList<QVector<QStringList>> tables = parser.ExtractInnerTables(content);

    buildTables(team, tables);
}

int DataManager::GetMaxColumnNumber()
{
    return _maxColumnNumber;
}

const DataManager::TeamTablesContainer &DataManager::GetTables()
{
    return _tables;
}

QList<TableStruct *> &DataManager::GetTables(QString team)
{
    return _tables[team];
}

void DataManager::buildTables(QString team, QList<QVector<QStringList> > &tables)
{
    QList<TableStruct*>& list = _tables[team];
    while(!tables.isEmpty())
    {
        QVector<QStringList> rows = tables.takeFirst();
        TableStruct* tablePtr = new TableStruct;
        buildRows(tablePtr, rows);
        list.push_back(tablePtr);
    }
}

void DataManager::buildRows(TableStruct* table, QVector<QStringList> &rows)
{
    foreach (auto row, rows)
    {
        Row r;
        foreach (auto column, row) {
            r.cells.push_back(HtmlParser::stripHtmlTags(column));
        }

        if (r.cells.first().contains("adsbygoogle"))
            continue;
        _maxColumnNumber = std::max(_maxColumnNumber, r.cells.size());
        table->rows.push_back(r);
    }
}

