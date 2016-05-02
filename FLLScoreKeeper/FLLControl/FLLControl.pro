#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T23:49:06
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FLLControl
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
        server.cpp \
    scoreentrydialog.cpp

HEADERS  += mainwindow.h \
        server.h \
    scoreentrydialog.h

FORMS    += mainwindow.ui \
    scoreentrydialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FLLShared/release/ -lFLLShared
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FLLShared/debug/ -lFLLShared
else:unix: LIBS += -L$$OUT_PWD/../FLLShared/ -lFLLShared

INCLUDEPATH += $$PWD/../FLLShared
DEPENDPATH += $$PWD/../FLLShared

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FLLShared/release/libFLLShared.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FLLShared/debug/libFLLShared.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FLLShared/release/FLLShared.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FLLShared/debug/FLLShared.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../FLLShared/libFLLShared.a

DISTFILES +=

RESOURCES +=
