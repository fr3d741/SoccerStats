#ifndef TABLESTRUCT_H
#define TABLESTRUCT_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QList>
#include <QString>

struct Row{
	QList<QString> cells;

	int maxColumnNumber();

	void Serialize(QXmlStreamWriter &stream);
	void Deserialize(QXmlStreamReader &stream);
};

class TableStruct
{
public:
	TableStruct();

	QList<Row> rows;

	int maxColumnNumber();

	void Serialize(QXmlStreamWriter &stream);
	void Deserialize(QXmlStreamReader &stream);
private:
};

#endif // TABLESTRUCT_H
