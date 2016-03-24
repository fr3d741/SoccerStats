#ifndef FILTER_H
#define FILTER_H

class IVisualizeFilter;
class Condition;
class Action;
class QWidget;

#include <QString>

class Filter{
		static int instanceCounter;
		int _id;
	protected:
		QString _name;
		IVisualizeFilter* _visualizer;
	public:
		Condition* preCondition;
		Action* action;

		Filter(
			QString name,
			Condition* preCondition,
			Action* action,
			IVisualizeFilter* visualizer);

		QString Name();

		int Id();

		virtual QWidget* GetVisual();
};

#endif // FILTER_H
