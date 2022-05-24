#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include <QObject>

class UartInterface : public QObject {
  Q_OBJECT

 public:
  explicit UartInterface();
 public slots:
  void station_answered(QByteArray);
};

#endif  // UARTINTERFACE_H
