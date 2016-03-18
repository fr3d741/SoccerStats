#ifndef FILTERFACADE_H
#define FILTERFACADE_H

class TableStruct;
class DataManager;

#include <QString>
#include <QMap>
#include <QList>
#include <QVariant>

#include "IVisualizeFilter.h"

struct Condition{
	virtual bool operator()(TableStruct *){return false;};
};

struct Result
{
	typedef QMap<QString, QVariant > ResultContainer;
	ResultContainer values;
};

class Action{
public:
	Result result;
	virtual void operator()(TableStruct *, QString){};
};


class Filter{
	protected:
		int _id;
		QString _name;
		IVisualizeFilter* _visualizer;
	public:
		Condition* preCondition;
		Action* action;

		Filter(
			int id,
			QString name,
			Condition* preCondition,
			Action* action,
			IVisualizeFilter* visualizer);

		QString Name();

		int Id();

		virtual QWidget* GetVisual();
};

class FilterFacade
{
		DataManager* _manager;
		QMap<int, Filter*> _filters;
	public:
		FilterFacade(DataManager* manager);

		Filter& ApplyFilter(int filterId);

		QList<Filter*> GetFilters();
};

#endif // FILTERFACADE_H
