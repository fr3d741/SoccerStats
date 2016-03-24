#ifndef MATCHFILTER_H
#define MATCHFILTER_H

#include <QTableWidgetItem>

#include "../Interfaces/Action.h"
#include "../Interfaces/IVisualizeFilter.h"
#include "../Filter.h"

class MatchFilter : public Filter
{
	std::shared_ptr<Result> _MatchResult;
private:
		class MatchesAction : public Action
		{
			public:

				MatchesAction(std::shared_ptr<Result> result);

				void operator()(std::shared_ptr<TableStruct> table, QString team);

			private:
				 void processRow(QString team, Row& row);

				 QPair<int, int> convertResult(QString str);

				 QMap<QString, QMap<QDate, QPair<int, int>>> _teamMatchResults;
		};

		class MatchTreeItem : public QTableWidgetItem
		{
			public:
				MatchTreeItem();

				MatchTreeItem(QString text);

				virtual bool operator<(const QTableWidgetItem& item) const;
		};

		class MatchVisualizer : public IVisualizeFilter
		{
				std::shared_ptr<Result> _result;
			public:
				MatchVisualizer(std::shared_ptr<Result> result);

				QWidget *GetDisplay();
			private:
				QTableWidgetItem* CreateTableWidgetItem(QMap<QString, QVariant>& map, QString tag);
		};

		MatchFilter(std::shared_ptr<Result> result);

public:
		static std::shared_ptr<Filter> CreateInstance();

};

#endif // MATCHFILTER_H
