#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>
#include <QMap>
#include <QString>

class HtmlParser
{
    QMap<QString, QMap<QString, QString> > teams;
public:
    typedef QMap<QString, QMap<QString, QString> > TeamsContainer;

	HtmlParser();

    bool parseTable(QString node, QString tableKey);

    QStringList extractLeagueLinks(QString html);

    void extractTeamLinks(QString html);

    QStringList extractValuesOf(QString key, QString in);

    QList<QVector<QStringList> > ExtractInnerTables(QString html);

    bool parseXML(QString file);

    TeamsContainer& getTeams();

public:
    static QString stripTags(QString str);

    static QString RemoveTagContent(QString html, QString tag);

    static QStringList stripHtmlTags(QStringList stringList);

    static QString stripHtmlTags(QString txt);

    static QString RemoveDuplicates(QString inText, QChar what);
private:
    QString getNextToken(QString html, int fromIndex);
    std::pair<QString, QString> recursiveTableExtract(QString html);

    /*
     * \brief returns <content, endTagIndex> pair
    */
    static std::pair<QString, int> getTag(QString tag, int from, QString in);

    /*
     * \brief returns <startIndex, endIndex> pair of tag
    */
    static std::pair<int, int> getTagIndices(QString tag, int from, QString in);

private:
    QVector<QStringList> parseRows(QString content);
    QList<QString> parseColumns(QString content);
};

#endif // HTMLPARSER_H
