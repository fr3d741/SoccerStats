#include <QTableWidget>

#include "visualizefilter.h"

VisualizeFilter::VisualizeFilter(std::shared_ptr<Result> filter)
	 :_result(filter)
{
}

QWidget *VisualizeFilter::GetDisplay()
{
	 QTableWidget* tableWidget = new QTableWidget;
	 tableWidget->setColumnCount(3);
	 tableWidget->setRowCount(_result->values.count());
	 tableWidget->setSortingEnabled(true);
	 int rowcount = 0;
	 for (Result::ResultContainer::iterator it = _result->values.begin(); it != _result->values.end(); ++it) {
		  QTableWidgetItem* key = new QTableWidgetItem(it.key());
		  QVariant val = it.value();
		  QPair<QString, QString> item = val.value<QPair<QString, QString>>();
		  QTableWidgetItem* value1 = new QTableWidgetItem(item.first);
		  QTableWidgetItem* value2 = new QTableWidgetItem(item.second);
		  tableWidget->setItem(rowcount, 0, key);
		  tableWidget->setItem(rowcount, 1, value1);
		  tableWidget->setItem(rowcount, 2, value2);
		  ++rowcount;
	 }

	 return tableWidget;
}
