QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawingprocessor.cpp \
    drawingsurface.cpp \
    main.cpp \
    controller.cpp \
    udp.cpp

HEADERS += \
    Constants.h \
    controller.h \
    drawingprocessor.h \
    drawingsurface.h \
    udp.h

FORMS += \
    controller.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
