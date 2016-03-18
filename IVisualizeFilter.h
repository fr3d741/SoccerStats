#ifndef IVISUALIZEFILTER_H
#define IVISUALIZEFILTER_H

#include <QWidget>

class IVisualizeFilter
{
public:

	 virtual QWidget *GetDisplay() = 0;
};

#endif // IVISUALIZEFILTER_H
