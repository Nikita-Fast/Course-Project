#ifndef UDP_H
#define UDP_H

#include <QObject>
#include "Constants.h"
#include <QUdpSocket>

class Udp : public QObject
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = nullptr);

signals:
    void bufferIsFull(short *buffer);

public slots:
    void readPendingDatagrams();
    void unlockBuffer(short *buffer);


private:
    short buffer1[BUFFER_SIZE];
    short buffer2[BUFFER_SIZE];
    int counter1 = 0, counter2 = 0;
    bool buffer1IsBlocked = false;
    bool buffer2IsBlocked = false;
    bool writeToBuffer1 = true;
    QUdpSocket *socket = nullptr;

    void packetToArray(const QByteArray packet, short *dst);
    void putSamplesToBuffer(short *samples, int length);
    std::pair<short *, int *> getBufferForWrite();
    void blockBuffer(short *buffer);
    static const int BOTH_BUFFERS_BLOCKED = 777;

};

#endif // UDP_H
