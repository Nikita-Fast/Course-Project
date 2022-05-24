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

  UartInterface uart;

  //  io::Uart* uart = new io::Uart("test");

  //  foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
  //    qDebug() << info.description();
  //    qDebug() << info.portName();
  //  }

  //  bool is_opened = uart->open();
  //  QByteArray bytes;
  //  bytes.resize(2);
  //  bytes[0] = 0x82;
  //  bytes[1] = 0x00;

  //  // bool suc = uart->write(bytes);
  //  qDebug() << suc;
  //  qDebug() << "opened = " << is_opened;

  return a.exec();
}
