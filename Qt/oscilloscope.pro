QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataprocessor.cpp \
    firfilter.cpp \
    main.cpp \
    mywindow.cpp \
    oscilloscope.cpp \
    screen.cpp \
    uartinterface.cpp \
    udpinterface.cpp \
    worker.cpp \
    io_uart.cpp \
    CRC16.cpp

HEADERS += \
    StrictRingBuffer.h \
    datainterface.h \
    dataprocessor.h \
    firfilter.h \
    mywindow.h \
    nonmoveable.h \
    oscilloscope.h \
    ringbuffer.h \
    screen.h \
    uartinterface.h \
    udpinterface.h \
    worker.h \
    io_uart.h \
    CRC16.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
