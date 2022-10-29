include(../../defaults.pri)
TEMPLATE = app

QT += testlib

SOURCES += main.cpp

LIBS += -L../../lib -loscill_lib
