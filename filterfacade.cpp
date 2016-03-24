#include <QDebug>
#include <QDate>
#include <QTableWidget>
#include <assert.h>

#include "filterfacade.h"
#include "tablestruct.h"
#include "datamanager.h"
#include "visualizefilter.h"
#include "Condition.h"
#include "Action.h"
#include "Result.h"

FilterFacade::FilterFacade(std::shared_ptr<DataManager> manager)
	 :_manager(manager)
{
	 assert(manager != nullptr);
}

Filter& FilterFacade::ApplyFilter(int filterId)
{
	 Filter& filter = *_filters[filterId];
	 DataManager::TeamTablesContainer teamTables = _manager->GetTables();
	 for(DataManager::TeamTablesContainer::iterator it = teamTables.begin(); it != teamTables.end(); ++it)
	 {
		  QList<std::shared_ptr<TableStruct>> tables = it.value();
		  QList<std::shared_ptr<TableStruct>> acceptedTables;
		  while(!tables.isEmpty())
		  {
				std::shared_ptr<TableStruct> table = tables.takeFirst();
				if ((*filter.preCondition)(table))
				{
					 acceptedTables.push_back(table);
				}
		  }

		  while(!acceptedTables.isEmpty())
		  {
				std::shared_ptr<TableStruct> table = acceptedTables.takeFirst();
				(*filter.action)(table, it.key());
		  }
	 }

	 return filter;
}

void FilterFacade::AddFilter(std::shared_ptr<Filter> filter)
{
	_filters[filter->Id()] = filter;
}

QList<std::shared_ptr<Filter> > FilterFacade::GetFilters()
{
	 return _filters.values();
}
