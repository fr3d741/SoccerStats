#include "filtercellsaction.h"

#include "../Interfaces/Result.h"

FilterCellsAction::FilterCellsAction(int r, int c)
	:Action(std::shared_ptr<Result>(new Result))
	,_row(r)
	,_column(c)
{
}

void FilterCellsAction::operator()(std::shared_ptr<TableStruct> table, QString team)
{
	auto rows = table->rows;
	if (rows.isEmpty())
		return;

	auto value = rows[_row].cells[_column];
	auto rowName = rows[_row].cells[0];
	_result->values[team] = QVariant::fromValue(QPair<QString, QString>(rowName,value));
}

