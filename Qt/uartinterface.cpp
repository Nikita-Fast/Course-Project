#include "uartinterface.h"
#include <QSerialPortInfo>
#include "io_uart.h"
#include "qdebug.h"

UartInterface::UartInterface() {
  foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
    qDebug() << info.description();

    io::Uart uart("/dev/ttyUSB0");

    // uart.setPortName(info.portName());

    qDebug() << uart.getPortName();
    qDebug() << "был открыт" << uart.isOpen();

    if (uart.open()) {
      qDebug() << "порт открыт!";
      QByteArray bytes;
      bytes.resize(2);
      bytes[0] = 0x82;
      bytes[1] = 0x00;

      if (uart.write(bytes)) {
        qDebug() << "данные посланы";
      } else {
        qDebug() << "метод write вернул false";
      }
    } else {
      qDebug() << "порт закрыт!";
    }
  }
}

void UartInterface::station_answered(QByteArray bytes) {
  unsigned char byte = bytes[0];
  if (byte == 0x82) {
    qDebug() << "станция ответила правильно";
  }
}
