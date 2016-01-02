#-------------------------------------------------
#
# Project created by QtCreator 2013-07-26T06:41:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pathfinding
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mapscene.cpp \
    astar.cpp \
    pathstepsthread.cpp

HEADERS  += mainwindow.h \
    mapscene.h \
    astar.h \
    pathstepsthread.h \
    fixinclude.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    images.qrc
