#include "filter.h"

#include <assert.h>

#include "Interfaces/IVisualizeFilter.h"

int Filter::instanceCounter = 0;

Filter::Filter(QString name, Condition* preCondition, Action* action, std::shared_ptr<IVisualizeFilter> visualizer)
	:_id(instanceCounter++)
	,_name(name)
	,_visualizer(visualizer)
	,preCondition(preCondition)
	,action(action)
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
