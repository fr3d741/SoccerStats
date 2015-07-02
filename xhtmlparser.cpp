#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "xhtmlparser.h"

XHtmlParser::XHtmlParser(QString file)
    :_file(file)
    ,_tags()
{
}

void XHtmlParser::Parse()
{
    QFile input(_file);
    input.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!input.isReadable())
    {
        qDebug() << input.errorString();
        return;
    }

    QTextStream stream(&input);
    _content = stream.readAll();
    GatherTags(_content);
    Node root;
    BuildDOM(&root, _content);
}

void XHtmlParser::GatherTags(QString content)
{
    int result = 0;
    int tag_close_idx = 0;
    QRegExp removeEx("[<>/]");
    while(result != -1)
    {
        result = content.indexOf("<", result);
        tag_close_idx = content.indexOf(">",result);
        QString tmp = content.mid(result, tag_close_idx - result + 1);
        QStringList qsl = tmp.split(" ");
        if (!qsl.isEmpty())
        {
            tmp = qsl.front();
        }
        tmp = tmp.remove(removeEx);
        qDebug() << "tmp: " << tmp;
        _tags.insert(tmp);
        result = tag_close_idx;
    }
}

void XHtmlParser::BuildDOM(Node* node, QString content)
{
    content = content.trimmed();
    int result = 0;
    int idx = 0;
    QString tag;

tryAgain:
    while(1)
    {
        Tag t = GetNextTag(content, result);
        if (t.state == State::Invalid)
            break;

        if (t.state == State::Broken)
        {
            qDebug() << "Broken state: " << content;
            ++result;
            continue;
        }

        if (!_tags.contains(t.tag))
        {
            qDebug() << "Unknown tag: " << t.tag;
            continue;
        }
        Node* n = new Node;
        n->name = t.tag;
        n->junk = content.mid(t.startTagIdx, t.innerStart - t.startTagIdx);
        node->children.append(n);
        if (t.innerStart != t.innerEnd)
        {
            QString innerC = content.mid(t.innerStart, t.innerEnd - t.innerStart);
            BuildDOM(n, innerC);
        }
        result = t.endTagIdx;
    }
}

Tag XHtmlParser::GetNextTag(QString content, int from) const
{
    QRegExp exp("/>|>");
    Tag resultTag;
    int start = content.indexOf("<", from);
    int end = content.indexOf(">", from);
    int firstSp = content.indexOf(" ", start);
    QString tag = content.mid(start + 1, firstSp - start - 1);

    int endTag = content.indexOf("</" + tag);
    int endTagI = content.indexOf(">", endTag);
    int closeTag = content.indexOf(exp, from);

    if (endTag == -1)
    {
        resultTag.innerEnd = resultTag.innerStart;
        resultTag.endTagIdx = closeTag + 2;
    }
    else
    {
        resultTag.innerEnd = endTag;
        resultTag.endTagIdx = endTagI;
    }
    resultTag.startTagIdx = start;
    resultTag.tag = tag;
    resultTag.innerStart = end + 1;
    if (start == -1)resultTag.state = State::Invalid;
    else if (endTag == -1 && closeTag == -1) resultTag.state = State::Broken;
    else resultTag.state = State::Valid;

return resultTag;
}
