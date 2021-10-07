#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = nullptr);

signals:

public slots:
    void readyReadMethod();

private:
    QUdpSocket *socket;
    int numberOfTransmissions;
    /*
    std::chrono::time_point<std::chrono::system_clock,
    std::chrono::duration<long, std::ratio<1, 1000000000>>> beginTime;*/
    std::chrono::time_point<std::chrono::high_resolution_clock> beginTime;

};

#endif // MYUDP_H
