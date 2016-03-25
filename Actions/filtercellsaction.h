#ifndef FILTERCELLSACTION_H
#define FILTERCELLSACTION_H

#include "../Interfaces/Action.h"

class FilterCellsAction : public Action
{
		int _row;
		int _column;
public:

	 FilterCellsAction(int r, int c);

	 void operator()(std::shared_ptr<TableStruct> table, QString team);
};

#endif // FILTERCELLSACTION_H
