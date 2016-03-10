#include <QDebug>
#include <QProcess>
#include <QDir>

#include "loaderthread.h"
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
#ifdef _DEBUG
	qDebug() << "start";
#endif
}

LoaderThread::~LoaderThread()
{
#ifdef _DEBUG
	qDebug() << "Destrukt";
#endif
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
#ifdef _DEBUG
    qDebug() << proc.readAllStandardOutput();

    qDebug() << dir;

    qDebug() << "Thread done";
#endif
    proc.waitForFinished();
    emit signalFinished();
    //proc.close();
}


