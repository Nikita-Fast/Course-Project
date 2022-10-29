include(../defaults.pri)

QT += widgets testlib

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../lib -loscill_lib
