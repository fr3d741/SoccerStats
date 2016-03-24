#ifndef FIRSTCELLCONDITION_H
#define FIRSTCELLCONDITION_H

#include <QString>
#include <memory>

#include "Condition.h"
#include "tablestruct.h"

class FirstCellCondition : public Condition
{
public:
	 QString cellValue;

	 FirstCellCondition(QString value);

	 bool operator()(std::shared_ptr<TableStruct> table);
};


#endif // FIRSTCELLCONDITION_H
