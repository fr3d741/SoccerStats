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
    _finishedThreads(),
    _loadedTeams(),
    _baseUrl("http://www.soccerstats.com/"),
    _pool(),
    _parser(),
    view(),
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
    QString url(_baseUrl + "leagues.asp");
    LoaderThread* thread = new LoaderThread(url, LoaderThread::StartPage);
    _pool = new QThreadPool();
    _pool->setMaxThreadCount( QThreadPool::globalInstance()->maxThreadCount() - 1 );
    thread->setAutoDelete(false);
    _pool->start(thread);
    _pool->waitForDone();

    QString html = readFile(thread->localFile());
    delete thread;

    QStringList links = _parser.extractLeagueLinks(html);
    ui->progressBar->setMaximum(links.size());
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setAlignment(Qt::AlignHCenter);
    ui->progressBar->setValue(0);
    while(!links.isEmpty())
    {
        QString link = links.takeFirst();
        LoaderThread* task = new LoaderThread(QString("%1%2").arg(_baseUrl).arg(link), LoaderThread::Leagues);
        connect(task, SIGNAL(signalFinished()), SLOT(slotLoadFinished()), Qt::QueuedConnection);
        task->setAutoDelete(false);
        _finishedThreads.push_back(task);
        _pool->start(task);
    }
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
    int value = ui->progressBar->value() + 1;
    int maxValue = ui->progressBar->maximum();
    ui->progressBar->setValue(value);
    ui->progressBar->update();
    //QApplication::processEvents();
    LoaderThread* sndr = dynamic_cast<LoaderThread*>(sender());
    switch(sndr->category)
    {
        case LoaderThread::StartPage:
            break;
        case LoaderThread::Leagues:
            {
                ui->progressBar->setFormat(QString("Downloading leagues...%1/%2").arg(value).arg(maxValue));
                _parser.extractTeamLinks(readFile(sndr->localFile()));
                QFile file(sndr->localFile());
                file.remove();
                file.setFileName("");
                if (!_pool->activeThreadCount())
                    LoadTeams();
            }
            break;
        case LoaderThread::Teams:
            {
                ui->progressBar->setFormat(QString("Downloading teams...%1/%2").arg(value).arg(maxValue));
                QString team = _loadedTeams[sndr];
                QString content = readFile(sndr->localFile());
                _manager->ParseHtml(team, content);
                QFile file(sndr->localFile());
                file.remove();
                if (!_pool->activeThreadCount())
                    LoadsFinished();
            }
            break;
    }
}

void MainWindow::LoadsFinished()
{
    ui->treeWidget->setColumnCount(_manager->GetMaxColumnNumber());
    QList<QString> teams = _manager->GetTables().keys();
    ui->listWidget->addItems(teams);
    int value = 0;
    int maxValue = teams.size();

    ui->progressBar->setMaximum(maxValue);
    while(!teams.isEmpty())
    {
        ui->progressBar->setValue(value);
        ui->progressBar->setFormat(QString("Filling tree...%1/2%").arg(value*100).arg(maxValue));
        QString team = teams.takeFirst();
        AddRootItemForTeam(team);
        value++;
    }
}

void MainWindow::LoadTeams()
{
    HtmlParser::TeamsContainer& teamLinks = _parser.getTeams();
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(teamLinks.size());
    for(HtmlParser::TeamsContainer::iterator it = teamLinks.begin(); it != teamLinks.end(); ++it)
    {
        QString link = it.value()["link"];
        LoaderThread* task = new LoaderThread(QString("%1%2").arg(_baseUrl).arg(link), LoaderThread::Teams);
        connect(task, SIGNAL(signalFinished()), SLOT(slotLoadFinished()), Qt::QueuedConnection);
        _loadedTeams[task] = it.key();
        task->setAutoDelete(false);
        _pool->start(task);
    }
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
