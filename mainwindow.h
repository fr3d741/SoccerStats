#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QWebView;
class QTreeWidgetItem;
class TableStruct;
class DataManager;
class FilterFacade;

#include <QMainWindow>
#include <QRunnable>
#include <QThreadPool>

#include <memory>

#include "htmlparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	 Ui::MainWindow *ui;
	 QList<QRunnable*> _finishedThreads;
	 QMap<QRunnable*, QString> _loadedTeams;
	 QString _baseUrl;
	 QThreadPool* _pool;
	 HtmlParser _parser;
	 QWebView* view;

	 DataManager* _manager;
	 FilterFacade* _filters;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	 void AddRootItemForTeam(QString team);
	 void LoadTeams();
	 void LoadsFinished();
	 void FillTeams();
private:
	void GatherTeams();
	 void AddTeamToTreeWidget(QString name, QList<QVector<QStringList> > &table);
	 void AddTableToTreeItem(QTreeWidgetItem* root, std::shared_ptr<TableStruct> table);
	 QString readFile(QString path) const;

public slots:
	 void slotLoadFinished();
private slots:
	void on_actionRefresh_triggered();

	 void on_actionFile_triggered();

	 void on_actionTables_triggered();

	 void on_ApplyFilter_pressed();

	 void on_actionSerialization_triggered();

	 void on_actionSave_triggered();

	 void on_actionLoad_triggered();

private:
	 void AddChildren(QVector<QStringList> &vector, QTreeWidgetItem *root);
	 void AddChildren(QStringList &vector, QTreeWidgetItem *root);
};

#endif // MAINWINDOW_H
