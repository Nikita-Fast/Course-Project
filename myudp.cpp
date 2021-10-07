#include "myudp.h"
#include <chrono>

MyUDP::MyUDP(QObject *parent) : QObject(parent)
{
    numberOfTransmissions = 0;
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 50001);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadMethod()));
}

bool isLast(QByteArray data) {
    bool res = true;
    for (int i = 0; i < 25; i++) {
        if ((int)data[i] != 127) {
            return false;
        }
    }
    return res;
}

void MyUDP::readyReadMethod() {
    qDebug() << "transitions = " << numberOfTransmissions;
    if (numberOfTransmissions == 0) {
        beginTime = std::chrono::high_resolution_clock::now();
        qDebug() << "-----------------";
    }

    QByteArray Buffer;
    QHostAddress sender;
    quint16 port;
    while (socket->hasPendingDatagrams()) {
        Buffer.resize(socket->pendingDatagramSize());
        socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &port);

        numberOfTransmissions++;

        qDebug() << "Msg from: " << sender.toString();
        qDebug() << "Msg port: " << port;
        qDebug() << "Msg: " << Buffer.toHex();
        /*
        auto nowTime = std::chrono::high_resolution_clock::now();
        qDebug() << "Time elapsed = " <<
                    std::chrono::duration_cast<std::chrono::milliseconds>(nowTime-beginTime).count()
                 << "ms";
            */

        if (isLast(Buffer)) {
            auto done = std::chrono::high_resolution_clock::now();
            auto finalTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-beginTime).count();
            qDebug() << "Time elapsed = " << finalTime<< "ms";
            qDebug() << "msg's received: " << numberOfTransmissions;

            auto receivedVolume = (double)Buffer.size() * (numberOfTransmissions - 1) / 1024;

            qDebug() << "Speed: " << receivedVolume / finalTime * 1000 << "KB/s";
            numberOfTransmissions = 0;
        }
    }
}
