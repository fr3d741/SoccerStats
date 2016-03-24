#include "filtercellsaction.h"

#include "Result.h"

FilterCellsAction::FilterCellsAction(int r, int c)
	:Action(std::shared_ptr<Result>(new Result))
	,row(r)
	,column(c)
{
}

void FilterCellsAction::operator()(std::shared_ptr<TableStruct> table, QString team)
{
	auto rows = table->rows;
	if (rows.isEmpty())
		return;

	auto value = rows[row].cells[column];
	auto rowName = rows[row].cells[0];
	_result->values[team] = QVariant::fromValue(QPair<QString, QString>(rowName,value));
}

