#include "matchesonlycondition.h"

MatchesOnlyCondition::MatchesOnlyCondition()
{
}

std::shared_ptr<Condition> MatchesOnlyCondition::CreateInstance()
{
	return std::shared_ptr<Condition>(new MatchesOnlyCondition);
}

bool MatchesOnlyCondition::operator()(std::shared_ptr<TableStruct> table)
{
	if (table->rows.isEmpty())
		return false;

	auto rows = table->rows;
	return rows[0].cells[0].contains("All results");
}
