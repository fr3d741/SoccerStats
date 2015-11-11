#include <QThreadPool>
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QTreeWidgetItem>
#include "mainwindow.h"
#include "htmlparser.h"
#include "loaderthread.h"
#include "tablestruct.h"
#include "xhtmlparser.h"

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
    QString baseUrl("http://www.soccerstats.com/");
    QString url(baseUrl + "leagues.asp");
    LoaderThread* thread = new LoaderThread(url);
    QThreadPool* pool = new QThreadPool();
    //pool->setMaxThreadCount(1);
    thread->setAutoDelete(false);
    pool->start(thread);
    pool->waitForDone();

    QString html = readFile(thread->localFile());
    HtmlParser& parser = _parser;
    delete thread;

    QStringList links = parser.extractLeagueLinks(html);

    while(!links.isEmpty())
    {
        QString link = links.takeFirst();
        LoaderThread* task = new LoaderThread(QString("%1%2").arg(baseUrl).arg(link));
        task->setAutoDelete(false);
        _finishedThreads.push_back(task);
        pool->start(task);
    }
    pool->waitForDone();

    while(!_finishedThreads.isEmpty())
    {
        LoaderThread* th = dynamic_cast<LoaderThread*>(_finishedThreads.takeFirst());
        parser.extractTeamLinks(readFile(th->localFile()));
        delete th;
    }

    ui->listWidget->addItems(parser.getTeams().keys());

    delete pool;
}

void MainWindow::ParseTeamStats()
{

}

QString MainWindow::readFile(QString path) const
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen() || !file.isReadable())
        return QString("");

    QTextStream stream(&file);
    return stream.readAll();
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

void MainWindow::on_actionFile_triggered()
{
    HtmlParser parser;
    QString file("12.t_html");
    QString content = readFile(file);

    parser.extractTeamLinks(content);
    QList<QVector<QStringList>> tables = parser.ExtractInnerTables(content);
    AddTeamToTreeWidget(file, tables);

    ui->listWidget->addItems(parser.getTeams().keys());

}

void MainWindow::AddTeamToTreeWidget(QString name, QList<QVector<QStringList> > &table)
{
    QTreeWidgetItem* root = new QTreeWidgetItem();
    root->setText(0, name);
    for( QList<QVector<QStringList> >::iterator it = table.begin(); it != table.end(); ++it )
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root);
        AddChildren(*it, item);
    }

    ui->treeWidget->addTopLevelItem(root);
}

void MainWindow::AddChildren(QVector<QStringList>& vector, QTreeWidgetItem* root)
{
    for(QVector<QStringList>::iterator it = vector.begin(); it != vector.end(); ++it)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root, *it);
    }
}
