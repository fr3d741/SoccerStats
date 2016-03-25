#ifndef FILTER_H
#define FILTER_H

class IVisualizer;
class Condition;
class Action;
class QWidget;

#include <QString>
#include <memory>

#include "tablestruct.h"

class Filter{
		static int instanceCounter;
		int _id;
	protected:
		QString _name;
		std::shared_ptr<IVisualizer> _visualizer;
		Condition* _preCondition;
		Action* _action;

	public:
		Filter(QString name,
				Condition* preCondition,
				Action* action,
				std::shared_ptr<IVisualizer> visualizer);

		QString Name();

		int Id();

		virtual QWidget* GetVisual();

		virtual bool PreCondition(std::shared_ptr<TableStruct> table);

		virtual void ExecuteActionOn(std::shared_ptr<TableStruct> table, QString team);
};

#endif // FILTER_H
