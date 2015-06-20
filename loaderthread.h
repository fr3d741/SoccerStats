#ifndef LOADERTHREAD_H
#define LOADERTHREAD_H

#include <QRunnable>
#include <QString>
#include <QWebView>

class LoaderThread : public QObject, public QRunnable
{
	Q_OBJECT
public:
	LoaderThread(QString url);
	virtual ~LoaderThread();

	QString localFile();

	void run();
signals:

public slots:

private:
	bool _stop;
	bool _loaded;
	QString _url;
	QString _local_file;

	static int _id;
};

#endif // LOADERTHREAD_H
