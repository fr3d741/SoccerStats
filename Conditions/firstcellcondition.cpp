#include "firstcellcondition.h"

FirstCellCondition::FirstCellCondition(QString value)
	:Condition()
	,_cellValue(value)
{

}

bool FirstCellCondition::operator()(std::shared_ptr<TableStruct> table)
{
	 if (table->rows.isEmpty())
		  return false;

	 auto rows = table->rows;
	 auto cells = rows.first().cells;
	 if (cells.isEmpty())
		  return false;

	 return cells.first().contains(_cellValue, Qt::CaseInsensitive);
}
