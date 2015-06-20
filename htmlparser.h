#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QWebElement>

struct Team{
	QString name;
	QString league;
	QMap< QString, float> values;
};

struct Table
{
	QMap<QString, QMap<QString, QString> > leagues;
	QMap<QString, QString> links;
};

extern QMap<QString, Team> teams;

class HtmlParser
{
public:
	HtmlParser();

	QMap<QString, QString> ExtractLinks(QWebElement node, QString key);

	bool parseTable(QWebElement node, QString tableKey);

    bool parseXML(QString file);

	static QString stripTags(QString str);

	QMap< QString, Table >& tables();
private:
	QMap< QString, Table > _tables;
};

#endif // HTMLPARSER_H
