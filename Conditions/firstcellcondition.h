#ifndef FIRSTCELLCONDITION_H
#define FIRSTCELLCONDITION_H

#include <QString>
#include <memory>

#include "../Interfaces/Condition.h"
#include "../tablestruct.h"

class FirstCellCondition : public Condition
{
    QString _cellValue;
public:
    FirstCellCondition(QString value);

    bool operator()(std::shared_ptr<TableStruct> table);
};


#endif // FIRSTCELLCONDITION_H
