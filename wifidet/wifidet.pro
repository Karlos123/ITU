#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T17:46:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wifidet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parsers.cpp \
    impexp.cpp \
    wifilist.cpp

HEADERS  += mainwindow.h \
    parsers.h \
    impexp.h \
    wifilist.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    sigicons.qrc
