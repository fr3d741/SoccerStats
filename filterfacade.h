#ifndef FILTERFACADE_H
#define FILTERFACADE_H

class TableStruct;
class DataManager;

#include <QString>
#include <QMap>
#include <QList>

struct Condition{
    virtual bool operator()(TableStruct *){return false;};
};

struct Result
{
    typedef QMap<QString, QPair<QString, QString> > ResultContainer;
    ResultContainer values;
};

struct Action{
    Result result;
    virtual void operator()(TableStruct *, QString){};
};


struct Filter{
    int Id;
    QString Name;
    Condition* preCondition;
    Action* action;
};

class FilterFacade
{
    DataManager* _manager;
    QMap<int, Filter> _filters;
public:
    FilterFacade(DataManager* manager);

    Filter& ApplyFilter(int filterId);

    QList<Filter> GetFilters();
};

#endif // FILTERFACADE_H
