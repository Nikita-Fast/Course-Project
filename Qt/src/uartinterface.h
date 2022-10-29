#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include <QObject>
#include "io_uart.h"
#include "datainterface.h"

class UartInterface : public DataInterface {
  Q_OBJECT

 public:
  explicit UartInterface();


 public slots:
  void station_answered(QByteArray);
  void error_response(QString msg);
  void sendDataToOscilloscope();

private:
  io::Uart *uart;
};

#endif  // UARTINTERFACE_H
