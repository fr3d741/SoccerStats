#include <QDebug>
#include <iostream>
#include "htmlparser.h"

HtmlParser::TeamsContainer &HtmlParser::getTeams()
{
    return teams;
}

QString HtmlParser::getNextToken(QString html, int fromIndex)
{
    QString stack;
    //int startIndex =
    for(int i = fromIndex; i < html.length(); ++i)
    {
        stack.append(html[i]);
    }

    return "";
}

std::pair<QString, QString> HtmlParser::recursiveTableExtract(QString html)
{
return std::pair<QString, QString>();
}

HtmlParser::HtmlParser()
{

}

bool HtmlParser::parseTable(QString node, QString tableKey)
{
    QRegExp rexp("[/>|>]");
    int tableIdx = node.indexOf("<table");
    int endtagIdx = node.indexOf(rexp, tableIdx);
    QString tableTag = node.mid(tableIdx, endtagIdx - tableIdx);
    qDebug() << tableTag;
    return false;
}

QStringList HtmlParser::extractLeagueLinks(QString html)
{
    QStringList links;
    QRegExp rexp("/>|>");
    QRegExp remExp("<|>|/");
    int nextPos = 0;

    while(1)
    {
        int linkBeginIdx1 = html.indexOf("<a", nextPos, Qt::CaseInsensitive);
        if (linkBeginIdx1==-1)
            break;
        int linkBeginIdx2 = html.indexOf(rexp, linkBeginIdx1);
        int endTag = nextPos = html.indexOf("</a",linkBeginIdx2, Qt::CaseInsensitive);
        QString linkTag = html.mid(linkBeginIdx1, linkBeginIdx2 - linkBeginIdx1);
        QString linkValue = html.mid(linkBeginIdx2 + 1,endTag - linkBeginIdx2 - 1);

        linkTag = linkTag.remove(remExp);
        if (!linkTag.contains("blacklink2", Qt::CaseInsensitive))
            continue;

        QStringList attributes = linkTag.split(" ");
        while(!attributes.isEmpty())
        {
            QString token = attributes.takeFirst();
            if (!token.contains("="))
                continue;

            //href="latest.asp?league=albania"
            int eqIdx = token.indexOf("=");
            QString key = token.mid(0, eqIdx);
            QString value = token.mid(eqIdx + 1);
            if (key!="href")
                continue;
            links.push_back(value.remove("'"));
        }
    }
    return links;
}

void HtmlParser::extractTeamLinks(QString html)
{
    QStringList resultList;
    QRegExp rexp("/>|>");
    QRegExp remExp("<|>|/");
    int nextPos = 0;

    while(1)
    {
        int trIdxBeg = html.indexOf("<a ", nextPos, Qt::CaseInsensitive);
        if (trIdxBeg==-1)
            break;

        int trIdxEnd = html.indexOf(rexp, trIdxBeg);
        int trEndTagIdx = nextPos = html.indexOf("</a", trIdxEnd, Qt::CaseInsensitive);
        QString tag = html.mid(trIdxBeg, trIdxEnd - trIdxBeg);
        QString val = html.mid(trIdxEnd + 1, trEndTagIdx - (trIdxEnd+1));
        QStringList attributes = tag.split(" ");
        while(!attributes.isEmpty())
        {
            QString node = attributes.takeFirst();
            if (!node.startsWith("href",Qt::CaseInsensitive) || !node.contains("team.asp"))
                continue;
            int idx = node.indexOf("=");
            QString href_val = node.mid(idx + 1);
            href_val = href_val.remove("\"").remove("\'");
            teams[val]["link"]=href_val;
        }
    }
}

QStringList HtmlParser::extractValuesOf(QString key, QString in)
{
    QRegExp rexp("/>|>");
    QStringList resultList;
    int idx = in.indexOf(key, Qt::CaseInsensitive);
    int termIdx = in.indexOf("<tr", idx, Qt::CaseInsensitive);
    while(idx<termIdx)
    {
        idx = in.indexOf("<td",idx, Qt::CaseInsensitive);
        if (termIdx <= idx)
            break;
        int tidx = in.indexOf(rexp, idx);
        int endIdx = idx = in.indexOf("</td",tidx, Qt::CaseInsensitive);
        QString result = in.mid(tidx+1, endIdx - (tidx+1));
        resultList.push_back(result);
    }
    return resultList;
}

QList<QVector<QStringList>> HtmlParser::ExtractInnerTables(QString html)
{
    QStringList tables;
    QString openTag("<table");
    QString closeTag("</table");
    QList<QVector<QStringList>> toret;
    int lastIndex = 0;
    while(1)
    {
        std::pair<QString,int> pair = getTag("table", lastIndex, html);
        if (pair.second == -1)
            break;
        lastIndex = pair.second;

        if (pair.first.contains(openTag, Qt::CaseInsensitive) || pair.first.contains(closeTag, Qt::CaseInsensitive))
            continue;

        QVector<QStringList> table = parseRows(pair.first);
        toret.append(table);
//        foreach (QStringList var, table) {
//            foreach (QString var2, var) {
//                std::cout << var2.toStdString();
//            }
//            std::cout << std::endl;
//        }
    }
    return toret;
}

QVector<QStringList> HtmlParser::parseRows(QString content)
{
    int lastIndex = 0;
    QVector<QStringList> rows;
    std::pair<QString,int> pair;

    while(pair.second != -1)
    {
        pair = getTag("tr", lastIndex, content);
        QStringList row = parseColumns(pair.first);
        if (!row.isEmpty())
            rows.push_back(row);
        lastIndex = pair.second;
    }
return rows;
}

QList<QString> HtmlParser::parseColumns(QString content)
{
    QList<QString> row;
    int lastIndex = 0;
    std::pair<QString,int> res;
    while(res.second != -1)
    {
        res = getTag("td", lastIndex, content);
        lastIndex = res.second;
        if (lastIndex == -1)
            break;

        row.push_back(res.first);
    }

    return row;
}

QString HtmlParser::stripTags(QString str)
{
	int in = 0;
	QString to_ret;
	foreach (QChar c, str) {
		if (c == '<')
			in++;
		else if (c == '>')
		{
			in--;
			continue;
		}

		if (!in)
			to_ret.append(c);
	}
    return to_ret;
}

QString HtmlParser::RemoveTagContent(QString html, QString tag)
{
    auto pair = getTagIndices(tag, 0, html);
    return html.remove(pair.first, pair.second - pair.first);
}

std::pair<QString,int> HtmlParser::getTag(QString tag, int from, QString in)
{
    QString expOpTag = QString("<%1").arg(tag);
    QString expClTag = QString("</%1").arg(tag);
    int clIndex = in.indexOf(expClTag, from, Qt::CaseInsensitive);
    int opIndex = in.lastIndexOf(expOpTag, clIndex, Qt::CaseInsensitive);

    if (opIndex == -1 || clIndex == -1 || clIndex <= opIndex)
        return std::make_pair("",-1);

    int endOfTag = in.indexOf(">", opIndex) + 1;
    return std::make_pair(in.mid(endOfTag, clIndex - endOfTag), clIndex + 1);
}

std::pair<int, int> HtmlParser::getTagIndices(QString tag, int from, QString in)
{
    QString expOpTag = QString("<%1").arg(tag);
    QString expClTag = QString("</%1").arg(tag);
    int clIndex = in.indexOf(expClTag, from, Qt::CaseInsensitive);
    int opIndex = in.lastIndexOf(expOpTag, clIndex, Qt::CaseInsensitive);

    if (opIndex == -1 || clIndex == -1 || clIndex <= opIndex)
        return std::make_pair(0,0);

    return std::make_pair(opIndex, clIndex + expClTag.size() + 1);
}

QStringList HtmlParser::stripHtmlTags(QStringList stringList)
{
    QStringList qsl;
    while(!stringList.isEmpty())
    {
        QString str = stringList.takeFirst();

        str = stripHtmlTags(str);
        if (str.length() == 0)
            continue;
        qsl.append(str);
    }

    return qsl;
}

QString HtmlParser::stripHtmlTags(QString txt)
{
    QString result;
    result = HtmlParser::RemoveTagContent(txt, "span");
    result = HtmlParser::stripTags(result);
    return RemoveDuplicates(result.replace("&nbsp;", " ").trimmed().replace("\n", " "), ' ');
}

QString HtmlParser::RemoveDuplicates(QString inText, QChar what)
{
    QString result;
    QChar lastChar;
    foreach (auto var, inText)
    {
        if (var == what && lastChar == what)
            continue;
        lastChar = var;
        result.append(var);
    }
    return result;
}
