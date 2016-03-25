#include <assert.h>

#include "filter.h"

#include "Interfaces/Action.h"
#include "Interfaces/Condition.h"
#include "Interfaces/IVisualizer.h"

int Filter::instanceCounter = 0;

Filter::Filter(QString name, Condition* preCondition, Action* action, std::shared_ptr<IVisualizer> visualizer)
	:_id(instanceCounter++)
	,_name(name)
	,_visualizer(visualizer)
	,_preCondition(preCondition)
	,_action(action)
{
	assert(preCondition != nullptr);
	assert(action != nullptr);
	assert(visualizer != nullptr);
}

QString Filter::Name()
{
	return _name;
}

int Filter::Id()
{
	return _id;
}

QWidget* Filter::GetVisual()
{
	return _visualizer->GetDisplay();
}

bool Filter::PreCondition(std::shared_ptr<TableStruct> table)
{
	return (*_preCondition)(table);
}

void Filter::ExecuteActionOn(std::shared_ptr<TableStruct> table, QString team)
{
	(*_action)(table, team);
}
