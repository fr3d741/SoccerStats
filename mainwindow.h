#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QWebView;

#include <QMainWindow>
#include <QWebElement>

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

public slots:
	void slotLoadFinished(bool);
private slots:
	void on_actionRefresh_triggered();

private:
	HtmlParser _parser;
	QWebView* view;
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
