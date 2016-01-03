#-------------------------------------------------
#
# Project created by QtCreator 2016-01-02T18:01:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = FLLShared
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += team.cpp \
        match.cpp \
        schedule.cpp

HEADERS += team.h \
        match.h \
        schedule.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
