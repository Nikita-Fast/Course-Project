#include <QSerialPortInfo>
#include <QtDebug>
#include "oscilloscope.h"

#include <QApplication>

#include "io_uart.h"
#include "uartinterface.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Oscilloscope w;
  w.show();

  UartInterface *uart = new UartInterface;

  return a.exec();
}
