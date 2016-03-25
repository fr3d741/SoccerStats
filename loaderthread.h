#ifndef LOADERTHREAD_H
#define LOADERTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QString>

class LoaderThread : public QObject, public QRunnable
{
	Q_OBJECT
public:
	enum Category{
		StartPage,
		Leagues,
		Teams
	};

	Category category;

	LoaderThread(QString url, Category type);
	virtual ~LoaderThread();

	QString localFile();

	virtual void run();
signals:
	 void signalFinished();

public slots:

private:
	bool _stop;
	bool _loaded;
	QString _url;
	QString _local_file;

	static int _id;
};

#endif // LOADERTHREAD_H
