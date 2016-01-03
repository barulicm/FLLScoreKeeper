#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T23:50:09
#
#-------------------------------------------------

QT       += core gui
QT += network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FLLDisplay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    client.h \
    optionsdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui

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
