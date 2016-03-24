#ifndef CONDITION_H
#define CONDITION_H

#include <memory>

#include "tablestruct.h"

class Condition{
public:
	virtual bool operator()(std::shared_ptr<TableStruct>){return false;}
};

#endif // CONDITION_H
