#include "controller.h"

#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller w;
    w.show();

    return a.exec();
}
