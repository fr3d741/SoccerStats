#include <QDebug>
#include <QDate>
#include "datamanager.h"

#include "htmlparser.h"
#include "tablestruct.h"

DataManager::DataManager()
	 :_maxColumnNumber(0)
{
}

void DataManager::ParseHtml(QString team, QString content)
{
	 HtmlParser parser;
	 QList<QVector<QStringList>> tables = parser.ExtractInnerTables(content);

	 buildTables(team, tables);
}

int DataManager::GetMaxColumnNumber()
{
	 int max = 0;
	 foreach (auto var, _tables) {
		  foreach (auto table, var) {
				max = std::max(table.get()->maxColumnNumber(), max);
		  }
	 }
	 return max;
}

QString DataManager::Serialize()
{
	 QString xml;
	 QXmlStreamWriter stream(&xml);
	 stream.setAutoFormatting(true);
	 stream.writeStartDocument();
	 stream.writeStartElement("Sample");
	 stream.writeAttribute("date", QDate::currentDate().toString());
	 {
		  stream.writeStartElement("Teams");
		  for(TeamTablesContainer::iterator it = _tables.begin(); it != _tables.end(); ++it)
		  {
				auto list = it.value();
				stream.writeStartElement("Team");
				stream.writeAttribute("name", it.key());
				stream.writeAttribute("tables", QString("%1").arg(it.value().size()));
				foreach(auto table, list)
				{
					 table->Serialize(stream);
				}
				stream.writeEndElement();
		  }
		  stream.writeEndElement();
	 }
	 stream.writeEndElement();
	 stream.writeEndDocument();
	 return xml;
}

void DataManager::Deserialize(QString content)
{
	 QXmlStreamReader reader(content);
	 //reader.readNext();
	 QXmlStreamReader::TokenType type = reader.readNext();
	 if (type != QXmlStreamReader::StartDocument)
		  return;

	 while(!reader.atEnd())
	 {
		  type = reader.readNext();
		  if (type == QXmlStreamReader::StartElement)
		  {
				if (reader.name() == "Team")
				{
					 auto attributes = reader.attributes();
					 if (!attributes.hasAttribute("name"))
						  continue;
					 QString teamName = attributes.value("name").toString();
					 QList<std::shared_ptr<TableStruct>>& tables = _tables[teamName];
					 bool ok = false;
					 int count = attributes.value("tables").toInt(&ok);
					 while(count)
					 {
						  QXmlStreamReader::TokenType subType = reader.readNext();
						  if (subType == QXmlStreamReader::StartElement)
						  {
								std::shared_ptr<TableStruct> table = std::shared_ptr<TableStruct>(new TableStruct);
								table.get()->Deserialize(reader);
								tables.push_back(table);
								count--;
						  }
					 }
				}
		  }
	 }
}

const DataManager::TeamTablesContainer &DataManager::GetTables()
{
	 return _tables;
}

QList<std::shared_ptr<TableStruct> >& DataManager::GetTables(QString team)
{
	 return _tables[team];
}

void DataManager::buildTables(QString team, QList<QVector<QStringList> > &tables)
{
	 QList<std::shared_ptr<TableStruct>>& list = _tables[team];
	 while(!tables.isEmpty())
	 {
		  QVector<QStringList> rows = tables.takeFirst();
		  std::shared_ptr<TableStruct> tablePtr = std::shared_ptr<TableStruct>(new TableStruct);
		  buildRows(tablePtr, rows);
		  list.push_back(tablePtr);
	 }
}

void DataManager::buildRows(std::shared_ptr<TableStruct> table, QVector<QStringList> &rows)
{
	 foreach (auto row, rows)
	 {
		  Row r;
		  foreach (auto column, row) {
				r.cells.push_back(HtmlParser::stripHtmlTags(column));
		  }

		  if (r.cells.first().contains("adsbygoogle"))
				continue;
		  _maxColumnNumber = std::max(_maxColumnNumber, r.cells.size());
		  table->rows.push_back(r);
	 }
}

