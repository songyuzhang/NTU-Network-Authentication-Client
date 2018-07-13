#-------------------------------------------------
#
# Project created by QtCreator 2014-09-16T16:19:23
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ntuClient1
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    icon.rc
RC_FILE += icon.rc
