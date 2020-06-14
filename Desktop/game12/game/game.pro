#-------------------------------------------------
#
# Project created by QtCreator 2020-05-13T20:00:17
#
#-------------------------------------------------

QT       += core gui multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TowerDefense
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    towerposition.cpp \
    tower.cpp \
    waypoint.cpp \
    enemy.cpp \
    bullet.cpp

HEADERS  += mainwindow.h \
    towerposition.h \
    tower.h \
    waypoint.h \
    enemy.h \
    bullet.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    change log.doc
