#ifndef FILTERFACADE_H
#define FILTERFACADE_H

class TableStruct;
class DataManager;

#include <QString>
#include <QMap>
#include <QList>

#include <memory>

#include "IVisualizeFilter.h"
#include "Filter.h"

class FilterFacade
{
		DataManager* _manager;
		QMap<int, std::shared_ptr<Filter>> _filters;
	public:
		FilterFacade(DataManager* manager);

		Filter& ApplyFilter(int filterId);

		void AddFilter(std::shared_ptr<Filter> filter);

		QList<std::shared_ptr<Filter>> GetFilters();
};

#endif // FILTERFACADE_H
