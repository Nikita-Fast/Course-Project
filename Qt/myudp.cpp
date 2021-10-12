#include "myudp.h"
#include <chrono>

MyUDP::MyUDP(QObject *parent, MainWindow *ptr) : QObject(parent)
{
    mainWindow = ptr;

    packetsReceived = 0;
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 50001);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadMethod()));
}

bool MyUDP::packetMarkedAsLast(const QByteArray &data) {
    bool res = true;
    int values[] = {-128, 0, 127};
    for (int i = 0; i < data.size(); i++) {
        if ((int)data[i] != values[i % 3]) {
            return false;
        }
    }
    return res;
}

void MyUDP::readyReadMethod() {
    //qDebug() << "transitions = " << packetsReceived;
    startMeasuringNetworkSpeed();

    QByteArray Buffer;
    while (socket->hasPendingDatagrams()) {
        Buffer.resize(socket->pendingDatagramSize());
        socket->readDatagram(Buffer.data(), Buffer.size());
        short samples[Buffer.size() / 2];
        packetToArray(Buffer, samples);
        packetsReceived++;
        //printSamples(samples, Buffer.size() / 2);

        if (!packetMarkedAsLast(Buffer)) {
            drawSamples(samples, Buffer.size() / 2, 0.005, packetsReceived - 1);
        }
        else {
            finishMeasuringNetworkSpeed(Buffer.size());
            packetsReceived = 0;
        }
    }
}

void MyUDP::drawSamples(short samples[], int length, double pixels_on_single_sample, int packets_already_drawn) {
    int scale = 32767 / (mainWindow -> getBlackboardHeight() / 2 - 75);
    for (int i = 0; i < length; i++) {
        int y = mainWindow -> getBlackboardHeight() / 2 - samples[i] / scale;

        int samples_drawn_before = packets_already_drawn * length + i;
        int x = samples_drawn_before * pixels_on_single_sample;

        QPoint point(x, y);
        mainWindow->addPoint(point);
    }
    mainWindow->update();
}

void MyUDP::printSamples(short array[], int length) {
    std::string str;
    for (int i = 0; i < length; i++) {
        str += (std::to_string(array[i]) + " ");
    }
    qDebug() << QString::fromStdString(str);
}

void MyUDP::finishMeasuringNetworkSpeed(const int& packet_size)
{
    auto done = std::chrono::high_resolution_clock::now();
    auto finalTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-beginTime).count();
    qDebug() << "Time elapsed = " << finalTime<< "ms";
    qDebug() << "msg's received: " << packetsReceived;

    auto receivedVolume = packet_size * (packetsReceived - 1) / 1024.0;
    qDebug() << "Speed: " << receivedVolume / finalTime * 1000 << "KB/s";
}

void MyUDP::startMeasuringNetworkSpeed()
{
    if (packetsReceived == 0) {
        beginTime = std::chrono::high_resolution_clock::now();
    }
}

void MyUDP::packetToArray(QByteArray packet, short array[])
{
    int size = packet.size();
    for (int i = 0; i < size; i += 2) {
        unsigned char b0 = packet[i];
        unsigned char b1 = packet[i + 1];
        array[i / 2] = (short)((b0 << 8) | b1);
    }
}
