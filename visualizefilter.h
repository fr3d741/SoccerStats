#ifndef VISUALIZEFILTER_H
#define VISUALIZEFILTER_H

#include <QWidget>

#include "filterfacade.h"

class VisualizeFilter
{
    Result& _result;
public:
    VisualizeFilter(Result& filter);

    QWidget *GetDisplay();
};

#endif // VISUALIZEFILTER_H
