#ifndef VISUALIZEFILTER_H
#define VISUALIZEFILTER_H

#include <QWidget>
#include <memory>

#include "Interfaces/IVisualizer.h"
#include "Interfaces/Result.h"

class VisualizeFilter : public IVisualizer
{
	 std::shared_ptr<Result> _result;
public:
	 VisualizeFilter(std::shared_ptr<Result> filter);

	 QWidget *GetDisplay();
};

#endif // VISUALIZEFILTER_H
