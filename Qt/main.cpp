#include <QtDebug>
#include "oscilloscope.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Oscilloscope w;
  w.show();

  qDebug() << 7 - (abs((-11)) % 7);

  return a.exec();
}
