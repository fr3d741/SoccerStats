#ifndef AUTOMATICTABLEBUILDER_H
#define AUTOMATICTABLEBUILDER_H

#include <QString>
#include <QList>

class AutomaticTableBuilder
{
    QString _source;
    QList<QString> _tags;
    int _index;
    int _stackPointer;
    char _stack[7];

public:
    AutomaticTableBuilder(QString src);

    void Process(QString str);

    QString CreateOpeningPattern();
    QString CreateClosingPattern();
private:
    bool Is(const char* str);
    void Push(char c);
    void ProcessColumn();
    void ProcessHeader();
    void ProcessRow();
    void ProcessSubtable();
    std::pair<int, int> GoToNextTag();
};

#endif // AUTOMATICTABLEBUILDER_H
