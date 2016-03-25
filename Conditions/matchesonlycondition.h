#ifndef MATCHESONLYCONDITION_H
#define MATCHESONLYCONDITION_H

#include "../Interfaces/Condition.h"

class MatchesOnlyCondition : public Condition
{
		MatchesOnlyCondition();
	public:
		static std::shared_ptr<Condition> CreateInstance();

		virtual bool operator()(std::shared_ptr<TableStruct> table);
};

#endif // MATCHESONLYCONDITION_H
