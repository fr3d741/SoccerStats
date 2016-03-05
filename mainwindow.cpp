#include <QThreadPool>
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QTreeWidgetItem>
#include "mainwindow.h"
#include "htmlparser.h"
#include "loaderthread.h"
#include "visualizefilter.h"
#include "datamanager.h"
#include "tablestruct.h"
#include "filterfacade.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow),
    _manager(new DataManager()),
    _filters(new FilterFacade(_manager))
{
	ui->setupUi(this);

    auto filters = _filters->GetFilters();
    while(!filters.isEmpty())
    {
        auto filter = filters.takeFirst();
        QListWidgetItem* item = new QListWidgetItem(filter.Name);
        item->setData(Qt::UserRole, QVariant::fromValue(filter.Id));
        ui->filtersListWidget->addItem(item);
    }

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
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(links.size());
    while(!links.isEmpty())
    {
        QString link = links.takeFirst();
        LoaderThread* task = new LoaderThread(QString("%1%2").arg(baseUrl).arg(link));
        connect(task, SIGNAL(signalFinished()), SLOT(slotLoadFinished()));
        task->setAutoDelete(false);
        _finishedThreads.push_back(task);
        pool->start(task);
    }
    pool->waitForDone();

    while(!_finishedThreads.isEmpty())
    {
        LoaderThread* th = dynamic_cast<LoaderThread*>(_finishedThreads.takeFirst());
        parser.extractTeamLinks(readFile(th->localFile()));
        QFile file(th->localFile());
        file.remove();
        delete th;
    }

    QMap<QString, QRunnable*> loadedTeams;
    HtmlParser::TeamsContainer& teamLinks = parser.getTeams();
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(teamLinks.size());
    for(HtmlParser::TeamsContainer::iterator it = teamLinks.begin(); it != teamLinks.end(); ++it)
    {
        QString link = it.value()["link"];
        LoaderThread* task = new LoaderThread(QString("%1%2").arg(baseUrl).arg(link));
        connect(task, SIGNAL(signalFinished()), SLOT(slotLoadFinished()));
        loadedTeams[it.key()] = task;
        task->setAutoDelete(false);
        pool->start(task);
    }

    pool->waitForDone();

    for(QMap<QString, QRunnable*>::iterator it = loadedTeams.begin(); it != loadedTeams.end(); ++it)
    {
        LoaderThread* task = dynamic_cast<LoaderThread*>(it.value());
        QString content = readFile(task->localFile());
        _manager->ParseHtml(it.key(), content);
        QFile file(task->localFile());
        file.remove();
    }

    ui->treeWidget->setColumnCount(_manager->GetMaxColumnNumber());
    QList<QString> teams = _manager->GetTables().keys();
    ui->listWidget->addItems(teams);
    while(!teams.isEmpty())
    {
        QString team = teams.takeFirst();
        AddRootItemForTeam(team);
    }

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

int MainWindow::calculateMaxNumberOfColumns(QList<QVector<QStringList> > &table)
{
    int max = 0;
    foreach(auto var, table)
    {
        foreach(auto var2, var)
        {
            max = std::max(var2.size(), max);
        }
    }
    return max;
}

void MainWindow::slotLoadFinished()
{
    int value = ui->progressBar->value();
    ui->progressBar->setValue(value + 1);
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

void MainWindow::AddTableToTreeItem(QTreeWidgetItem *root, TableStruct *table)
{
    foreach (auto var, table->rows) {
        QTreeWidgetItem* item = new QTreeWidgetItem(root, var.cells);
    }
}

void MainWindow::AddChildren(QVector<QStringList>& vector, QTreeWidgetItem* root)
{
    for(QVector<QStringList>::iterator it = vector.begin(); it != vector.end(); ++it)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root, HtmlParser::stripHtmlTags(*it));
    }
}

void MainWindow::AddRootItemForTeam(QString team)
{
    QTreeWidgetItem* root = new QTreeWidgetItem();
    root->setText(0, team);

    foreach (auto var, _manager->GetTables(team)) {
        QTreeWidgetItem* row = new QTreeWidgetItem(root);
        AddTableToTreeItem(row, var);
    }

    ui->treeWidget->addTopLevelItem(root);
}

void MainWindow::on_actionTables_triggered()
{
    HtmlParser parser;
    QString file("test.htm");
    QString content = readFile(file);

    _manager->ParseHtml("test", content);
    ui->treeWidget->setColumnCount(_manager->GetMaxColumnNumber());
    AddRootItemForTeam("test");
}

void MainWindow::on_ApplyFilter_pressed()
{
    QList<QListWidgetItem*> items = ui->filtersListWidget->selectedItems();
    while(!items.isEmpty())
    {
        QListWidgetItem* item = items.takeFirst();
        bool ok = false;
        int id = item->data(Qt::UserRole).toInt(&ok);
        if (ok)
        {
            auto result = _filters->ApplyFilter(id);
            VisualizeFilter visual(result.action->result);
            ui->resultTabs->addTab(visual.GetDisplay(), item->text());
        }
    }
}
