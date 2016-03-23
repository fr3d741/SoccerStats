#include <QDebug>
#include <QDate>
#include <QTableWidget>
#include <assert.h>

#include "filterfacade.h"
#include "tablestruct.h"
#include "datamanager.h"
#include "visualizefilter.h"

struct FirstCellCondition : Condition{
	 QString cellValue;

	 FirstCellCondition(QString value)
	 {
		  cellValue = value;
	 }

	 bool operator()(std::shared_ptr<TableStruct> table)
	 {
		  if (table->rows.isEmpty())
				return false;

		  auto rows = table->rows;
		  auto cells = rows.first().cells;
		  if (cells.isEmpty())
				return false;

		  return cells.first().contains(cellValue, Qt::CaseInsensitive);
	 }
};

class FilterCellsAction : public Action
{
public:
	 int row;
	 int column;
	 FilterCellsAction(int r, int c)
		  :row(r)
		  ,column(c)
	 {
	 }

	 void operator()(std::shared_ptr<TableStruct> table, QString team)
	 {
		  auto rows = table->rows;
		  if (rows.isEmpty())
				return;

		  auto value = rows[row].cells[column];
		  auto rowName = rows[row].cells[0];
		  result.values[team] = QVariant::fromValue(QPair<QString, QString>(rowName,value));
	 }
};

class ScoringAction : public Action{
public:
	 ScoringAction()
	 {
	 }

	 void operator()(std::shared_ptr<TableStruct> table, QString team)
	 {
		  auto rows = table->rows;
		  if (rows.isEmpty())
				return;

		  auto value = rows[1].cells[3];
		  auto rowName = rows[1].cells[0];
		  result.values[team] = QVariant::fromValue(QPair<QString, QString>(rowName,value));
	 }
};

const QString homeTag = "Goals@Home";
const QString awayTag = "Goals@Away";
const QString avgHomeTag = "Avg. " + homeTag;
const QString avgAwayTag = "Avg. " + awayTag;

class MatchesAction : public Action{
public:

	MatchesAction()
	{
	}

	void operator()(std::shared_ptr<TableStruct> table, QString team)
	{
		auto rows = table->rows;
		if (rows.isEmpty())
			return;

		foreach (auto row, rows) {
			processRow(team, row);
		}

		QMap<QString, QVariant> statistic;
		QMap<QDate, QPair<int, int>>& matches = _teamMatchResults[team];
		bool ok;
		foreach (auto item, matches) {
			QPair<int, int> pair = item;
			int home = statistic[homeTag].toInt(&ok) + pair.first;
			int away = statistic[awayTag].toInt(&ok) + pair.second;
			statistic[homeTag] = QVariant::fromValue(home);
			statistic[awayTag] = QVariant::fromValue(away);
		}

		{
			float home = statistic[homeTag].toFloat(&ok) / (float)matches.count();
			float away = statistic[awayTag].toFloat(&ok) / (float)matches.count();

			statistic[avgHomeTag] = QVariant::fromValue(home);
			statistic[avgAwayTag] = QVariant::fromValue(away);
		}

		result.values[team] = QVariant::fromValue(statistic);
	}

private:
	 void processRow(QString team, Row& row)
	 {
		QDate date = QDate::fromString(row.cells[0], "d MMM");
		if (row.cells.size() < 4)
			return;

		QString home = row.cells[1];
		QString guest = row.cells[3];
		QString resultString = row.cells[2];
		QPair<int, int> result = convertResult(resultString);
		if (result.first < 0 || result.second < 0)
			return;
		QPair<int, int> matchResult;
		if (home.contains(team))
			matchResult = result;
		else if (guest.contains(team))
		{
			matchResult.first = result.second;
			matchResult.second = result.first;
		}

		_teamMatchResults[team][date] = matchResult;
	 }

	 QPair<int, int> convertResult(QString str)
	 {
		 QPair<int, int> result = QPair<int, int>(-1, -1);
		 bool ok;
		 QStringList list = str.split('-');
		 if (list.size() < 2)
			 return result;
		 result.first = list[0].toInt(&ok);
		 if (!ok) result.first = -1;
		 result.second = list[1].toInt(&ok);
		 if (!ok) result.second = -1;

		 return result;
	 }

	 QMap<QString, QMap<QDate, QPair<int, int>>> _teamMatchResults;
};

class MatchTreeItem : public QTableWidgetItem
{
public:
		MatchTreeItem()
			:QTableWidgetItem()
		{
		}

		MatchTreeItem(QString text)
			:QTableWidgetItem(text)
		{
		}

		virtual bool operator<(const QTableWidgetItem& item) const
		{
			bool okItem;
			bool okMine;
			float itemValue = item.text().toFloat(&okItem);
			float mineValue = text().toFloat(&okMine);
			if (okItem && okMine)
			{
				return mineValue < itemValue;
			}

			return QTableWidgetItem::operator<(item);
		}
};

class MatchVisualizer : public IVisualizeFilter
{
		Result& _result;
	public:
		MatchVisualizer(Result& filter)
			:_result(filter)
		{
		}

		QWidget *GetDisplay()
		{
			QTableWidget* tableWidget = new QTableWidget;

			const int homeIndex = 1;
			const int awayIndex = 2;
			const int avgHomeIndex = 3;
			const int avgAwayIndex = 4;
			tableWidget->setColumnCount(5);
			tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Team"));
			tableWidget->setHorizontalHeaderItem(homeIndex, new MatchTreeItem(homeTag));
			tableWidget->setHorizontalHeaderItem(awayIndex, new MatchTreeItem(awayTag));
			tableWidget->setHorizontalHeaderItem(avgHomeIndex, new MatchTreeItem(avgHomeTag));
			tableWidget->setHorizontalHeaderItem(avgAwayIndex, new MatchTreeItem(avgAwayTag));

			tableWidget->setRowCount(_result.values.count());
			tableWidget->setSortingEnabled(true);
			int rowcount = 0;
			for (Result::ResultContainer::iterator it = _result.values.begin(); it != _result.values.end(); ++it) {
				QVariant val = it.value();
				QMap<QString, QVariant> item = val.value<QMap<QString, QVariant>>();

				tableWidget->setItem(rowcount, 0, new QTableWidgetItem(it.key()));
				tableWidget->setItem(rowcount, homeIndex, CreateTableWidgetItem(item, homeTag));
				tableWidget->setItem(rowcount, awayIndex, CreateTableWidgetItem(item, awayTag));
				tableWidget->setItem(rowcount, avgHomeIndex, CreateTableWidgetItem(item, avgHomeTag));
				tableWidget->setItem(rowcount, avgAwayIndex, CreateTableWidgetItem(item, avgAwayTag));
				++rowcount;
			}

			return tableWidget;
		}
private:
	QTableWidgetItem* CreateTableWidgetItem(QMap<QString, QVariant>& map, QString tag)
	{
		QTableWidgetItem* item = new MatchTreeItem();
		bool ok;
		QVariant v = map[tag];
		float value = v.toFloat(&ok);
		item->setText(QString("%1").arg(value));
		return item;
	}
};

FilterFacade::FilterFacade(DataManager *manager)
	 :_manager(manager)
{
	 assert(manager != nullptr);

	 auto scoringAction = new ScoringAction();
	 _filters[0] = std::shared_ptr<Filter>(new Filter(0, "TestFilter", new FirstCellCondition("SCORING"), scoringAction, new VisualizeFilter(scoringAction->result)));
	 auto cellAction = new FilterCellsAction(1,1);
	 _filters[1] = std::shared_ptr<Filter>(new Filter(1, "Goals scored per game", new FirstCellCondition("Goals scored per game"), cellAction, new VisualizeFilter(cellAction->result)));
	 auto maction = new MatchesAction();
	 _filters[2] = std::shared_ptr<Filter>(new Filter(2, "Matches", new FirstCellCondition("All results (chronological order)"), maction, new MatchVisualizer(maction->result)));
}

Filter& FilterFacade::ApplyFilter(int filterId)
{
	 Filter& filter = *_filters[filterId];
	 DataManager::TeamTablesContainer teamTables = _manager->GetTables();
	 for(DataManager::TeamTablesContainer::iterator it = teamTables.begin(); it != teamTables.end(); ++it)
	 {
		  QList<std::shared_ptr<TableStruct>> tables = it.value();
		  QList<std::shared_ptr<TableStruct>> acceptedTables;
		  while(!tables.isEmpty())
		  {
				std::shared_ptr<TableStruct> table = tables.takeFirst();
				if ((*filter.preCondition)(table))
				{
					 acceptedTables.push_back(table);
				}
		  }

		  while(!acceptedTables.isEmpty())
		  {
				std::shared_ptr<TableStruct> table = acceptedTables.takeFirst();
				(*filter.action)(table, it.key());
		  }
	 }

	 return filter;
}

QList<std::shared_ptr<Filter> > FilterFacade::GetFilters()
{
	 return _filters.values();
}


Filter::Filter(int id, QString name, Condition* preCondition, Action* action, IVisualizeFilter* visualizer)
	:_id(id)
	,_name(name)
	,_visualizer(visualizer)
	,preCondition(preCondition)
	,action(action)
{
	assert(preCondition != nullptr);
	assert(action != nullptr);
	assert(visualizer != nullptr);
}

QString Filter::Name()
{
	return _name;
}

int Filter::Id()
{
	return _id;
}

QWidget* Filter::GetVisual()
{
	return _visualizer->GetDisplay();
}
