//#include "teamextractor.h"

//#include "htmlparser.h"

//TeamExtractor::TeamExtractor()
//{

//}

//void TeamExtractor::run()
//{
//    LoaderThread::run();

//    HtmlParser parser;
//    QString html = readFile(localFile());
//    parser.extractTeamLinks(html);
//}

//QString TeamExtractor::readFile(QString path) const
//{
//    QFile file(path);
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    if (!file.isOpen() || !file.isReadable())
//        return QString("");

//    QTextStream stream(&file);
//    return stream.readAll();
//}
