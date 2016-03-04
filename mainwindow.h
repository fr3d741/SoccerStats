#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QWebView;
class QTreeWidgetItem;
class TableStruct;

#include <QMainWindow>
#include <QWebElement>
#include <QRunnable>

#include "htmlparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void GatherTeams();
    void ParseTeamStats();
    void AddTeamToTreeWidget(QString name, QList<QVector<QStringList> > &table);
    void AddTableToTreeItem(QTreeWidgetItem* root, TableStruct* table);
    QString readFile(QString path) const;
    int calculateMaxNumberOfColumns(QList<QVector<QStringList> > &table);

public slots:
	void slotLoadFinished(bool);
private slots:
	void on_actionRefresh_triggered();

    void on_actionFile_triggered();

    void on_actionTables_triggered();

private:
    QList<QRunnable*> _finishedThreads;
	HtmlParser _parser;
	QWebView* view;
    Ui::MainWindow *ui;
    void AddChildren(QVector<QStringList> &vector, QTreeWidgetItem *root);
    void AddChildren(QStringList &vector, QTreeWidgetItem *root);
};

#endif // MAINWINDOW_H
