#ifndef FILTERCELLSACTION_H
#define FILTERCELLSACTION_H

#include "Action.h"

class FilterCellsAction : public Action
{
public:
	 int row;
	 int column;

	 FilterCellsAction(int r, int c);

	 void operator()(std::shared_ptr<TableStruct> table, QString team);
};

#endif // FILTERCELLSACTION_H
