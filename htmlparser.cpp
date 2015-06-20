#include "htmlparser.h"


QMap<QString, Team> teams = QMap<QString, Team>();


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
