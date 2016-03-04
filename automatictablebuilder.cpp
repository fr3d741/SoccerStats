#include <QDebug>
#include "automatictablebuilder.h"

AutomaticTableBuilder::AutomaticTableBuilder(QString src)
    :_source(src)
    ,_index(0)
    ,_stackPointer(0)
{
    _tags.push_back("td");
    _tags.push_back("th");
    _tags.push_back("tr");
    _tags.push_back("table");
}

void AutomaticTableBuilder::Process(QString str)
{
    QString stack;
    for(int i = 0; i < str.length(); ++i)
    {
        Push(str.at(i).toLatin1());
        if (Is("<t"))
        {

        }
    }
}

QString AutomaticTableBuilder::CreateOpeningPattern()
{
    QString openingPattern;
    for(int i = 0; i < _tags.length(); ++i)
    {
        if (i)
        {
            openingPattern.append("|");
        }
        openingPattern.append("<" + _tags[i]);
    }

    return openingPattern;
}

QString AutomaticTableBuilder::CreateClosingPattern()
{
    QString closingPattern;
    for(int i = 0; i < _tags.length(); ++i)
    {
        if (i)
        {
            closingPattern.append("|");
        }
        closingPattern.append("</" + _tags[i]);
    }

    return closingPattern;
}

bool AutomaticTableBuilder::Is(const char *str)
{
    int i = 0;
    while(1)
    {
        if (str[i]=='\0' || _stack[i]=='\0' || i == 6)
            break;
        if (str[i] != _stack[i])
            return false;
        ++i;
    }

    return str[i]=='\0' && _stack[i]=='\0';
}

void AutomaticTableBuilder::Push(char c)
{
    if (_stackPointer == 6)
    {
        for (int i = 0; i < 6; ++i)
            _stack[i] = _stack[i + 1];
    }
    _stack[_stackPointer] = c;

    _stackPointer = std::min(6, ++_stackPointer);
}

std::pair<int,int> AutomaticTableBuilder::GoToNextTag()
{
    QString openingPattern = CreateOpeningPattern();
    QString closingPattern = CreateClosingPattern();
    QRegExp exp( openingPattern + closingPattern );

    int tokenIdx = _source.indexOf(exp, _index);
    int closingIdx = _source.indexOf(">", tokenIdx);

    return std::make_pair(tokenIdx, closingIdx);
}
