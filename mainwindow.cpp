#include <QThreadPool>
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include "mainwindow.h"
#include "htmlparser.h"
#include "loaderthread.h"
#include "tablestruct.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
//	QUrl url("http://www.soccerstats.com/leagues.asp");
//	ui->setupUi(this);
//	view = new QWebView(this);
//	view->setVisible(false);
//	connect(view, SIGNAL(loadFinished(bool)), SLOT(slotLoadFinished(bool)));
//	view->load(url);

//    QXmlStreamReader reader()
//    while (!reader.atEnd()) {
//        reader.readNext();
//        if (reader.isStartElement()) {
//            if (reader.name() == "title")
//                title = reader.readElementText();
//            else if(reader.name() == "a")
//                links.append(reader.attributes().value("href").toString());
//            else if(reader.name() == "p")
//                ++paragraphCount;
//        }
//    }
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::GatherTeams()
{
    QString url("http://www.soccerstats.com/leagues.asp");
    LoaderThread* thread = new LoaderThread(url);
    QThreadPool* pool = new QThreadPool();
    thread->setAutoDelete(false);
    pool->start(thread);
    pool->waitForDone();

    QString curent = QDir::currentPath();
    QDir d(curent);
    QStringList list = d.entryList();

    QFile file(thread->localFile());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug() << file.errorString();
        QXmlStreamReader reader(&file);
    while(!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            reader.readNext();
            if (reader.name() == "table")
            {
                TableStruct ts(reader);
                QList<Row> r = ts.rows;
            }
        }
        else
            reader.readNext();
    }

    delete pool;
}

void MainWindow::slotLoadFinished(bool)
{
	//qDebug() << "Loaded **** ";
//	QWebPage* page = view->page();
//	QWebElementCollection collection = page->mainFrame()->findAllElements("table");

//	foreach (QWebElement e, collection) {
//		if (_parser.parseTable(e, "Leagues"))
//			break;
//	}


//	Table t = _parser.tables()["Leagues"];
//	LoaderThread worker("http://www.soccerstats.com/" + t.links.begin().value());
//	worker.run();
}

void MainWindow::on_actionRefresh_triggered()
{
    GatherTeams();
}
