#ifndef SCORINGACTION_H
#define SCORINGACTION_H

#include "Action.h"

class ScoringAction : public Action
{
public:
	 ScoringAction();

	 void operator()(std::shared_ptr<TableStruct> table, QString team);
};

#endif // SCORINGACTION_H
