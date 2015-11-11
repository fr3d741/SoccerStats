#include <QDebug>
#include <iostream>
#include "htmlparser.h"


//QMap<QString, Team> teams = QMap<QString, Team>();



QMap<QString, QMap<QString, QString> > &HtmlParser::getTeams()
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

QMap<QString, QString> HtmlParser::ExtractLinks(QWebElement node, QString key)
{
	QStringList qsl = node.attributeNames();
//	foreach (var, qsl) {
//		//if ()
//	}
return QMap<QString, QString>();
}

bool HtmlParser::parseTable(QWebElement node, QString tableKey)
{
	QWebElement e = node.firstChild();
	if (e.localName() != "thead")
		return false;

	QWebElement em = e.findFirst("th");
	if ( stripTags(em.toInnerXml()) != tableKey)
		return false;

	Table table;

	QMap< int, QString > keymapping;
	int idx = 0;
	QWebElementCollection headElements = e.findAll("th");
	foreach (QWebElement a, headElements) {
		QWebElement b = a.findFirst("span");
		keymapping[idx++] = stripTags(b.toInnerXml());
	}

	QWebElementCollection rows = node.findAll("tr");
	foreach(QWebElement row, rows)
	{
		QWebElementCollection cols = row.findAll("td");
		//QString innner = row.toInnerXml();
		int idx = 0;
		QString key;
		foreach (QWebElement td, cols) {
			if (idx == 0)
			{
				QWebElement a = td.firstChild();
				table.links[a.toInnerXml()] = a.attribute("href", "");
				key = a.toInnerXml();
				idx++;
			}
			else
			{
				table.leagues[key][keymapping[idx++]] = td.toInnerXml();
			}
		}

	}

	_tables[tableKey] = table;
    return true;
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
        int linkBeginIdx1 = html.indexOf("<a", nextPos);
        if (linkBeginIdx1==-1)
            break;
        int linkBeginIdx2 = html.indexOf(rexp, linkBeginIdx1);
        int endTag = nextPos = html.indexOf("</a",linkBeginIdx2);
        QString linkTag = html.mid(linkBeginIdx1, linkBeginIdx2 - linkBeginIdx1);
        QString linkValue = html.mid(linkBeginIdx2 + 1,endTag - linkBeginIdx2 - 1);

        linkTag = linkTag.remove(remExp);
        if (!linkTag.contains("blacklink2"))
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
        int trIdxBeg = html.indexOf("<a ", nextPos);
        if (trIdxBeg==-1)
            break;

        int trIdxEnd = html.indexOf(rexp, trIdxBeg);
        int trEndTagIdx = nextPos = html.indexOf("</a", trIdxEnd);
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
            href_val = href_val.remove("\"");
            teams[val]["link"]=href_val;
        }
    }
}

QStringList HtmlParser::extractValuesOf(QString key, QString in)
{
    QRegExp rexp("/>|>");
    QStringList resultList;
    int idx = in.indexOf(key);
    int termIdx = in.indexOf("<tr", idx);
    while(idx<termIdx)
    {
        idx = in.indexOf("<td",idx);
        if (termIdx <= idx)
            break;
        int tidx = in.indexOf(rexp, idx);
        int endIdx = idx = in.indexOf("</td",tidx);
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

        if (pair.first.contains(openTag) || pair.first.contains(closeTag))
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

QMap<QString, Table> &HtmlParser::tables()
{
    return _tables;
}

void HtmlParser::parseTeamRow(Team &t, QString row)
{
    QStringList columns = row.split("<td");

}

std::pair<QString,int> HtmlParser::getTag(QString tag, int from, QString in)
{
    QString expOpTag = QString("<%1").arg(tag);
    QString expClTag = QString("</%1").arg(tag);
    int clIndex = in.indexOf(expClTag, from);
    int opIndex = in.lastIndexOf(expOpTag, clIndex);

    if (opIndex == -1 || clIndex == -1 || clIndex <= opIndex)
        return std::make_pair("",-1);

    int endOfTag = in.indexOf(">", opIndex) + 1;
    return std::make_pair(in.mid(endOfTag, clIndex - endOfTag), clIndex + 1);
}
