#include <QDebug>
#include <assert.h>

#include "filterfacade.h"
#include "tablestruct.h"
#include "datamanager.h"

struct FirstCellCondition : Condition{
    QString cellValue;

    FirstCellCondition(QString value)
    {
        cellValue = value;
    }

    bool operator()(TableStruct* table)
    {
        if (table->rows.isEmpty())
            return false;

        auto rows = table->rows;
        auto cells = rows.first().cells;
        if (cells.isEmpty())
            return false;

        return cells.first().contains(cellValue, Qt::CaseInsensitive);
    }
};

struct ScoringAction : Action{

    ScoringAction()
    {
    }

    void operator()(TableStruct* table, QString team)
    {
        auto rows = table->rows;
        if (rows.isEmpty())
            return;

        auto value = rows[1].cells[3];
        auto rowName = rows[1].cells[0];
        result.values[team] = QPair<QString, QString>(rowName,value);
    }
};


FilterFacade::FilterFacade(DataManager *manager)
    :_manager(manager)
{
    assert(manager != nullptr);

    _filters[0] = Filter{0, "TestFilter", new FirstCellCondition("SCORING"), new ScoringAction()};
}

Filter& FilterFacade::ApplyFilter(int filterId)
{
    Filter& filter = _filters[filterId];
    QMap<QString, QList<TableStruct *> > teamTables = _manager->GetTables();
    for(QMap<QString, QList<TableStruct *> >::iterator it = teamTables.begin(); it != teamTables.end(); ++it)
    {
        QList<TableStruct *> tables = it.value();
        QList<TableStruct *> acceptedTables;
        while(!tables.isEmpty())
        {
            TableStruct* table = tables.takeFirst();
            if ((*filter.preCondition)(table))
            {
                acceptedTables.push_back(table);
            }
        }

        while(!acceptedTables.isEmpty())
        {
            TableStruct* table = acceptedTables.takeFirst();
            (*filter.action)(table, it.key());
        }
    }

    return filter;
}

QList<Filter> FilterFacade::GetFilters()
{
    return _filters.values();
}
