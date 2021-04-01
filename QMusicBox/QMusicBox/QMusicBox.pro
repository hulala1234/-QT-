#-------------------------------------------------
#
# Project created by QtCreator 2020-02-19T09:41:24
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += network
QT       += sql
RC_ICONS = logo.ico


include(./sqlapi/sqlapi.pri)
INCLUDEPATH     += $$PWD/sqlapi

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMusicBox
TEMPLATE = app


SOURCES += main.cpp\
        musicwidget.cpp \
    downloadwidget.cpp \
    customwidget.cpp

HEADERS  += musicwidget.h \
    downloadwidget.h \
    customwidget.h

FORMS    += musicwidget.ui \
    downloadwidget.ui

RESOURCES += \
    resource.qrc
