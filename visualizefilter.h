#ifndef VISUALIZEFILTER_H
#define VISUALIZEFILTER_H

#include <QWidget>

#include "IVisualizeFilter.h"
#include "filterfacade.h"

class VisualizeFilter : public IVisualizeFilter
{
	 Result& _result;
public:
	 VisualizeFilter(Result& filter);

	 QWidget *GetDisplay();
};

#endif // VISUALIZEFILTER_H
