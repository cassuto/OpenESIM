QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = esim-ide
TEMPLATE = app

CONFIG += console

include(schematic/schematic.pri)
include(persistence/persistence.pri)
include(util/util.pri)

SOURCES += \
    mainwindow.cc \
    workspace.cc \
    workspaceschematic.cc \
    main.cc
    
HEADERS  += \
    mainwindow.h \
    workspace.h \
    workspaceschematic.h

RESOURCES += resources.qrc

QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-parameter
