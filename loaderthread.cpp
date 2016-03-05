#include "loaderthread.h"
#include <QWebFrame>
#include <QProcess>
#include <QDir>

#include "htmlparser.h"

int LoaderThread::_id = 0;

LoaderThread::LoaderThread(QString url, Category type)
	:QObject(0)
	,QRunnable()
	,_stop(false)
	,_loaded(false)
	,_url(url)
    ,_local_file(QString("%1.t_html").arg(++_id))
{
    category = type;
//	connect(_view, SIGNAL(loadFinished(bool)), SLOT(slotLoaded(bool)));
//	connect(_view, SIGNAL(loadProgress(int)), SLOT(slotProgress(int)));
	qDebug() << "start";
}

LoaderThread::~LoaderThread()
{
	qDebug() << "Destrukt";
}

QString LoaderThread::localFile()
{
	return _local_file;
}

void LoaderThread::run()
{
	QProcess proc;
	QStringList parameters;

	parameters << "load.pl " << _url << _local_file;
	QString dir = QDir::currentPath();
	proc.setWorkingDirectory(dir);
	proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.start("perl.exe ", parameters);
    bool result = proc.waitForFinished(-1);

    qDebug() << proc.readAllStandardOutput();

//    while(1)
//    {
//        QFile f(_local_file);
//        if (f.exists())
//            break;
//    }

    qDebug() << dir;

    qDebug() << "Thread done";
    emit signalFinished();
}


