#-------------------------------------------------
#
# Project created by QtCreator 2012-03-11T18:52:29
#
#-------------------------------------------------
LIBS+=-L/usr/lib/kde -lkdeui
LIBS += `pkg-config opencv --cflags --libs`
include(/usr/include/qextserialport/src/qextserialport.pri)

QT       += core gui webkit

TARGET = load
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    funzioni.cpp

HEADERS  += mainwindow.h \
    funzioni.h

FORMS    += mainwindow.ui
