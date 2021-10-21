#include "udp.h"

Udp::Udp(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, PORT);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Udp::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray packet;
        packet.resize(socket->pendingDatagramSize());
        socket->readDatagram(packet.data(), packet.size());

        short samples[packet.size() / 2];
        packetToArray(packet, samples);

        putSamplesToBuffer(samples, packet.size() / 2);
    }
}

void Udp::packetToArray(const QByteArray packet, short *dst)
{
    int size = packet.size();
    for (int i = 0; i < size; i += 2) {
        unsigned char b0 = packet[i];
        unsigned char b1 = packet[i + 1];
        dst[i / 2] = (short)((b0 << 8) | b1);
    }
}

void printSamples(short array[], int length) {
    std::string str;
    for (int i = 0; i < length; i++) {
        str += (std::to_string(array[i]) + " ");
    }
    qDebug() << QString::fromStdString(str);
}

void Udp::putSamplesToBuffer(short *samples, int samplesNumber)
{
    std::pair<short *, int *> pair = getBufferForWrite();
    short *buffer = pair.first;
    int *counter = pair.second;

    //теряем данные если размер буфера не кратен размеру пакета
    int length = std::min(samplesNumber, BUFFER_SIZE - (*counter));
    memcpy(buffer + *counter, samples, length * sizeof(short));
    *counter += samplesNumber;

    if (*counter >= BUFFER_SIZE) {
        writeToBuffer1 = !writeToBuffer1;
        blockBuffer(buffer);
        //printSamples(buffer, BUFFER_SIZE);
        emit(bufferIsFull(buffer));
    }
}

std::pair<short *, int *> Udp::getBufferForWrite()
{
    if (writeToBuffer1 && !buffer1IsBlocked) {
        return std::pair<short *, int *>(buffer1, &counter1);
    }
    if (!writeToBuffer1 && !buffer2IsBlocked) {
        return std::pair<short *, int *>(buffer2, &counter2);
    }
    exit(BOTH_BUFFERS_BLOCKED);
}

void Udp::blockBuffer(short *buffer)
{
    if (buffer == buffer1) {
        buffer1IsBlocked = true;
        return;
    }
    if (buffer == buffer2) {
        buffer2IsBlocked = true;
        return;
    }
    exit(10);
}

void Udp::unlockBuffer(short *buffer)
{
    if (buffer == buffer1) {
        counter1 = 0;
        buffer1IsBlocked = false;
    }
    if (buffer == buffer2) {
        counter2 = 0;
        buffer2IsBlocked = false;
    }
}
