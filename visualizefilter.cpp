#include <QTableWidget>

#include "visualizefilter.h"

VisualizeFilter::VisualizeFilter(Result &filter)
    :_result(filter)
{
}

QWidget *VisualizeFilter::GetDisplay()
{
    QTableWidget* tableWidget = new QTableWidget;
    tableWidget->setColumnCount(3);
    tableWidget->setRowCount(_result.values.count());
    int rowcount = 0;
    for (Result::ResultContainer::iterator it = _result.values.begin(); it != _result.values.end(); ++it) {
        QTableWidgetItem* key = new QTableWidgetItem(it.key());
        QTableWidgetItem* value1 = new QTableWidgetItem(it.value().first);
        QTableWidgetItem* value2 = new QTableWidgetItem(it.value().second);
        tableWidget->setItem(rowcount, 0, key);
        tableWidget->setItem(rowcount, 1, value1);
        tableWidget->setItem(rowcount, 2, value2);
        ++rowcount;
    }

    return tableWidget;
}
