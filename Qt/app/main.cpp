#include <QApplication>
#include "oscilloscope.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Oscilloscope w;
  w.show();

  return a.exec();
}
