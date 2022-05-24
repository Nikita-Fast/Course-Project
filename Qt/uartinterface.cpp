#include "uartinterface.h"
#include <QSerialPortInfo>
#include "io_uart.h"
#include "qdebug.h"

UartInterface::UartInterface() {
    auto info = QSerialPortInfo::availablePorts().first();

    uart = new io::Uart(info.portName());

    uart->setPortName(info.portName());

    connect(uart, SIGNAL(packetReceived(QByteArray)), this, SLOT(station_answered(QByteArray)));
    connect(uart, SIGNAL(error(QString)), this, SLOT(error_response(QString)));

    if (uart->open()) {
      qDebug() << "порт открыт!";
      QByteArray bytes;
      bytes.resize(2);
      bytes[0] = 0x82;
      bytes[1] = 0x00;

//      if (uart->write(bytes)) {
//        qDebug() << "данные посланы";

//      } else {
//        qDebug() << "метод write вернул false";
//      }
    } else {
      qDebug() << "порт закрыт!";
    }

}

void UartInterface::station_answered(QByteArray bytes) {
//    qDebug() << "станция ответила";
//  unsigned char byte = bytes[0];
//  if (byte == 0x82) {
//    qDebug() << "станция ответила правильно";
//  }

    //qDebug() << bytes.size();

    int samples_number = (bytes.size() - 2) / 2;
    short samples[samples_number];

    for (int i = 2; i < bytes.size(); i += 2) {
      unsigned char b0 = bytes[i];
      unsigned char b1 = bytes[i + 1];
      samples[(i - 2) / 2] = (short)((b0 << 8) | b1);
//      qDebug() << samples[(i - 2) / 2];
    }
    emit(packetSent(samples, samples_number));
}

void UartInterface::error_response(QString msg)
{
    qDebug() << msg;
}

void UartInterface::sendDataToOscilloscope() {}

