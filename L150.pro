#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T16:36:39
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = L150
TEMPLATE = app


QMAKE_CXXFLAGS += -std=gnu++11

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    obrabotka.h \
    glgraf.h \
    glskal.h \
    grafik.h \
    parametr.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    obrabotka.cpp \
    glgraf.cpp \
    glskal.cpp \
    grafik.cpp \
    parametr.cpp

INCLUDEPATH = c:/Qt/Proj/graf
