#ifndef RULE_H
#define RULE_H

#include <QVariant>

class Rule{
public:
		virtual bool operator()(QVariant item) = 0;
};

#endif // RULE_H
