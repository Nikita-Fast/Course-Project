#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include "mainwindow.h"

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = nullptr, MainWindow *ptr = nullptr);

signals:

public slots:
    void readyReadMethod();

private:
    void drawSamples(short samples[], int length, double pixels_on_single_sample, int packet_number);
    void packetToArray(QByteArray packet, short array[]);
    void printSamples(short array[], int length);
    QUdpSocket *socket;
    MainWindow *mainWindow;
    int packetsReceived;
    std::chrono::time_point<std::chrono::high_resolution_clock> beginTime;
    void finishMeasuringNetworkSpeed(const int& packet_size);
    void startMeasuringNetworkSpeed();
    bool packetMarkedAsLast(const QByteArray &data);

};

#endif // MYUDP_H
