#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "datainterface.h"

class UdpInterface : public DataInterface {
  Q_OBJECT
 public:
  explicit UdpInterface();

 public slots:
  void sendDataToOscilloscope();

 private:
  QUdpSocket* socket = nullptr;
  void packetToArray(const QByteArray packet, short* dst);
};

#endif  // UDPINTERFACE_H
