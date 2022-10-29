include(../defaults.pri)
QT       += core gui widgets network serialport

TEMPLATE = lib
TARGET = oscill_lib

SOURCES += \
    dataprocessor.cpp \
    firfilter.cpp \
    moving_average_flter.cpp \
    oscilloscope.cpp \
    screen.cpp \
    uartinterface.cpp \
    udpinterface.cpp \
    io_uart.cpp \
    CRC16.cpp

HEADERS += \
    StrictRingBuffer.h \
    datainterface.h \
    dataprocessor.h \
    firfilter.h \
    moving_average_filter.h \
    nonmoveable.h \
    oscilloscope.h \
    screen.h \
    uartinterface.h \
    udpinterface.h \
    io_uart.h \
    CRC16.h
