#-------------------------------------------------
#
# Project created by QtCreator 2023-09-20T15:03:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = Project
TEMPLATE = app

INCLUDEPATH += ../include/qwt
INCLUDEPATH += ../include/xlsx

DEFINES += QT_DEPRECATED_WARNINGS

win32 {
    LIBS += -L"../lib"
}
CONFIG(debug, debug|release) {
    LIBS += -lqwtd
    LIBS += -lxlsxd
    DESTDIR = ../debug
} else {
    LIBS += -lqwt
    LIBS += -lxlsx
    DESTDIR = ../release
}


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    calc.h \
    signals.h

FORMS += \
        mainwindow.ui


