#ifndef VISUALIZEFILTER_H
#define VISUALIZEFILTER_H

#include <QWidget>
#include <memory>

#include "IVisualizeFilter.h"
#include "Result.h"

class VisualizeFilter : public IVisualizeFilter
{
	 std::shared_ptr<Result> _result;
public:
	 VisualizeFilter(std::shared_ptr<Result> filter);

	 QWidget *GetDisplay();
};

#endif // VISUALIZEFILTER_H
