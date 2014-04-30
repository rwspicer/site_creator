#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T09:03:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = site_creator
TEMPLATE = app


SOURCES += main.cpp\
        site_creator.cpp \
    paramfile.cpp \
    strmap.cpp

HEADERS  += site_creator.h \
    csvrow.h \
    paramfile.h \
    strmap.h

FORMS    += site_creator.ui
QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    data_types.txt \
    unit_maps.txt
