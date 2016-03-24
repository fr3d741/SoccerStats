#ifndef FILTERFACADE_H
#define FILTERFACADE_H

class TableStruct;
class DataManager;

#include <QString>
#include <QMap>
#include <QList>

#include <memory>

#include "Interfaces/IVisualizeFilter.h"
#include "Filter.h"

class FilterFacade
{
		std::shared_ptr<DataManager> _manager;
		QMap<int, std::shared_ptr<Filter>> _filters;
	public:
		FilterFacade(std::shared_ptr<DataManager> manager);

		Filter& ApplyFilter(int filterId);

		void AddFilter(std::shared_ptr<Filter> filter);

		QList<std::shared_ptr<Filter>> GetFilters();
};

#endif // FILTERFACADE_H
