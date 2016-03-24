#ifndef ACTION_H
#define ACTION_H

#include <memory>

#include "Result.h"
#include "../tablestruct.h"

class Action
{
protected:
	std::shared_ptr<Result> _result;
public:
	Action(std::shared_ptr<Result> resultPtr)
		:_result(resultPtr){
	}

	virtual void operator()(std::shared_ptr<TableStruct>, QString) = 0;

	std::shared_ptr<Result> result(){
		return _result;
	}
};

#endif // ACTION_H
