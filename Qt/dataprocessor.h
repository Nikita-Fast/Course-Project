#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr);

public slots:
    void storePacket(short *packet, int length);
    void sendDataToScreen(int amount);

signals:
    void sendSamples(short *ptrToValues, int amount);

private:
    static const int buffer_size = 12800;
    short processorBuffer[buffer_size];
    int counter1 = -1; //индекс последнего сэмпла
    int counter2 = 0; //индекс сэмпла (начала группы сэмплов), который будет отправлен экрану при ближайшем запросе

};

#endif // DATAPROCESSOR_H
