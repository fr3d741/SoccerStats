#ifndef XHTMLPARSER_H
#define XHTMLPARSER_H

#include <QString>
#include <QSet>
#include <QList>

enum class State
{
    Invalid,
    Valid,
    Broken
};

struct Tag
{
    QString tag;
    int startTagIdx;
    int endTagIdx;
    int innerStart;
    int innerEnd;
    State state;
};

struct Node
{
    QString name;
    QString junk;
    QList<Node*> children;
};

class XHtmlParser
{
    QString _file;
    QString _content;

    //html tags in document
    QSet<QString> _tags;
public:
    XHtmlParser(QString file);

    void Parse();
private:
    void GatherTags(QString content);
    void BuildDOM(Node* node, QString content);
    Tag GetNextTag(QString content, int from) const;
};

#endif // XHTMLPARSER_H
