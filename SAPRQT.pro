#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T17:56:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SAPRQT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    draw.cpp \
    saveload.cpp

HEADERS  += mainwindow.h \
    draw.h \
    saveload.h

FORMS    += mainwindow.ui
