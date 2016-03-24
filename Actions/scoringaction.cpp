#include "scoringaction.h"

ScoringAction::ScoringAction()
	:Action(std::shared_ptr<Result>(new Result))
{
}

void ScoringAction::operator()(std::shared_ptr<TableStruct> table, QString team)
{
	auto rows = table->rows;
	if (rows.isEmpty())
		return;

	auto value = rows[1].cells[3];
	auto rowName = rows[1].cells[0];
	_result->values[team] = QVariant::fromValue(QPair<QString, QString>(rowName,value));
}

