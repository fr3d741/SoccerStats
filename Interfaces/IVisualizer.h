#ifndef IVISUALIZEFILTER_H
#define IVISUALIZEFILTER_H

#include <QWidget>

class IVisualizer
{
public:

	 virtual QWidget *GetDisplay() = 0;
};

#endif // IVISUALIZEFILTER_H
