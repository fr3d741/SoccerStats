#ifndef RESULT_H
#define RESULT_H

#include <QMap>
#include <QString>
#include <QVariant>

class Result
{
public:
	typedef QMap<QString, QVariant> ResultContainer;
	ResultContainer values;
};

#endif // RESULT_H
