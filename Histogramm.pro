#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T15:00:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Histogramm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    histogramview.cpp \
    histogramdata.cpp \
    rgbhistogramdata.cpp

HEADERS  += mainwindow.h \
    histogramview.h \
    histogramdata.h \
    rgbhistogramdata.h

FORMS    += mainwindow.ui
