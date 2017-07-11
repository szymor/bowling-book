#-------------------------------------------------
#
# Project created by QtCreator 2013-07-14T17:58:18
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bowling
TEMPLATE = app


SOURCES += main.cpp\
        datechoicewindow.cpp \
    formwindow.cpp \
    datamanager.cpp \
    blacklistwindow.cpp

HEADERS  += datechoicewindow.h \
    formwindow.h \
    datamanager.h \
    blacklistwindow.h

FORMS    += datechoicewindow.ui \
    formwindow.ui \
    blacklistwindow.ui
