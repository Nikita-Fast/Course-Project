#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>

class DataInterface : public QObject{
    Q_OBJECT

signals:
    void packetSent(short *packet, int length);

public slots:
    virtual void sendDataToOscilloscope() = 0;

};

#endif // DATAINTERFACE_H
