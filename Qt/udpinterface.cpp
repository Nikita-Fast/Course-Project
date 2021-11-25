#include "udpinterface.h"
#include "QtDebug"

UdpInterface::UdpInterface()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 50001);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sendDataToOscilloscope()));
}

void UdpInterface::sendDataToOscilloscope()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray packet;
        packet.resize(socket->pendingDatagramSize());
        socket->readDatagram(packet.data(), packet.size());

        short samples[packet.size() / 2];
        packetToArray(packet, samples);

        emit(packetSent(samples, packet.size() / 2));

    }
}

void UdpInterface::packetToArray(const QByteArray packet, short *dst)
{
    int size = packet.size();
    for (int i = 0; i < size; i += 2) {
        unsigned char b0 = packet[i];
        unsigned char b1 = packet[i + 1];
        dst[i / 2] = (short)((b0 << 8) | b1);
    }
}


