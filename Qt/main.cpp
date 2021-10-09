#include "mainwindow.h"

#include <QApplication>
#include "myudp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MyUDP signalProcessor(nullptr, &w);
    signalProcessor.readyReadMethod();

    return a.exec();
}
