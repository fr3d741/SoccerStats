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
    visualizefilter.cpp

HEADERS  += mainwindow.h \
    loaderthread.h \
    htmlparser.h \
    tablestruct.h \
	datamanager.h \
    filterfacade.h \
    visualizefilter.h \
    IVisualizeFilter.h

FORMS    += mainwindow.ui
