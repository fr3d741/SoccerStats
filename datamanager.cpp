#include "datamanager.h"

#include "htmlparser.h"
#include "tablestruct.h"

DataManager::DataManager()
    :_maxColumnNumber(0)
{
}

void DataManager::ParseHtml(QString content)
{
    HtmlParser parser;
    QList<QVector<QStringList>> tables = parser.ExtractInnerTables(content);

    buildTables(tables);
}

int DataManager::GetMaxColumnNumber()
{
    return _maxColumnNumber;
}

QList<TableStruct *> &DataManager::GetTables()
{
    return _tables;
}

void DataManager::buildTables(QList<QVector<QStringList> > &tables)
{
    while(!tables.isEmpty())
    {
        QVector<QStringList> rows = tables.takeFirst();
        TableStruct* tablePtr = new TableStruct;
        buildRows(tablePtr, rows);
        _tables.push_back(tablePtr);
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

