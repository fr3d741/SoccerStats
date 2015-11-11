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

//extern QMap<QString, Team> teams;

class HtmlParser
{
    QMap<QString, QMap<QString, QString> > teams;
public:
	HtmlParser();

	QMap<QString, QString> ExtractLinks(QWebElement node, QString key);

	bool parseTable(QWebElement node, QString tableKey);

    bool parseTable(QString node, QString tableKey);

    QStringList extractLeagueLinks(QString html);

    void extractTeamLinks(QString html);

    QStringList extractValuesOf(QString key, QString in);

    QList<QVector<QStringList> > ExtractInnerTables(QString html);

    bool parseXML(QString file);

	static QString stripTags(QString str);

	QMap< QString, Table >& tables();

    QMap<QString, QMap<QString, QString> >& getTeams();

private:
    QString getNextToken(QString html, int fromIndex);
    std::pair<QString, QString> recursiveTableExtract(QString html);
    void parseTeamRow(Team& t, QString row);
    std::pair<QString, int> getTag(QString tag, int from, QString in);

private:
    QMap< QString, Table > _tables;
    QVector<QStringList> parseRows(QString content);
    QList<QString> parseColumns(QString content);
};

#endif // HTMLPARSER_H
