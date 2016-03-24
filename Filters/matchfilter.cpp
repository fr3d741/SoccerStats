#include <QDate>

#include "matchfilter.h"

#include "../Conditions/firstcellcondition.h"

const QString homeTag = "Goals@Home";
const QString awayTag = "Goals@Away";
const QString avgHomeTag = "Avg. " + homeTag;
const QString avgAwayTag = "Avg. " + awayTag;


MatchFilter::MatchesAction::MatchesAction(std::shared_ptr<Result> result)
	:Action(result)
{
}

void MatchFilter::MatchesAction::operator()(std::shared_ptr<TableStruct> table, QString team)
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
	foreach (auto item, matches)
	{
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

	_result->values[team] = QVariant::fromValue(statistic);
}

void MatchFilter::MatchesAction::processRow(QString team, Row& row)
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

QPair<int, int> MatchFilter::MatchesAction::convertResult(QString str)
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

MatchFilter::MatchTreeItem::MatchTreeItem()
	:QTableWidgetItem()
{
}

MatchFilter::MatchTreeItem::MatchTreeItem(QString text)
	:QTableWidgetItem(text)
{
}

bool MatchFilter::MatchTreeItem::operator<(const QTableWidgetItem& item) const
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

MatchFilter::MatchVisualizer::MatchVisualizer(std::shared_ptr<Result> result)
	:_result(result)
{
}

QWidget* MatchFilter::MatchVisualizer::GetDisplay()
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

	tableWidget->setRowCount(_result->values.count());
	tableWidget->setSortingEnabled(true);
	int rowcount = 0;
	for (Result::ResultContainer::iterator it = _result->values.begin(); it != _result->values.end(); ++it) {
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

QTableWidgetItem* MatchFilter::MatchVisualizer::CreateTableWidgetItem(QMap<QString, QVariant>& map, QString tag)
{
	QTableWidgetItem* item = new MatchTreeItem();
	bool ok;
	QVariant v = map[tag];
	float value = v.toFloat(&ok);
	item->setText(QString("%1").arg(value));
	return item;
}



MatchFilter::MatchFilter(std::shared_ptr<Result> result)
	:Filter("Matches", new FirstCellCondition("All results (chronological order)"), new MatchesAction(result), std::shared_ptr<IVisualizeFilter>(new MatchVisualizer(result)))
	,_MatchResult(result)
{
}

std::shared_ptr<Filter> MatchFilter::CreateInstance()
{
	return std::shared_ptr<Filter>(new MatchFilter(std::shared_ptr<Result>(new Result)));
}
