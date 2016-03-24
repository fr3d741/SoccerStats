#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T18:36:36
#
#-------------------------------------------------

QT       += webkitwidgets network core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebGetter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loaderthread.cpp \
    htmlparser.cpp \
    tablestruct.cpp \
	datamanager.cpp \
    filterfacade.cpp \
    visualizefilter.cpp \
    filter.cpp \
    firstcellcondition.cpp \
    filtercellsaction.cpp \
    scoringaction.cpp \
    Filters/matchfilter.cpp

HEADERS  += mainwindow.h \
    loaderthread.h \
    htmlparser.h \
    tablestruct.h \
	datamanager.h \
    filterfacade.h \
    visualizefilter.h \
    IVisualizeFilter.h \
    filter.h \
    Condition.h \
    Result.h \
    Action.h \
    firstcellcondition.h \
    filtercellsaction.h \
    scoringaction.h \
    Filters/matchfilter.h

FORMS    += mainwindow.ui
