#include "dataprocessor.h"
#include "QtDebug"
#include <QTimer>

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
    buffTimer = new QTimer;
    connect(buffTimer, SIGNAL(timeout()), this, SLOT(sendDataToScreen()));
}

DataProcessor::~DataProcessor()
{
    delete buffTimer;
}

void DataProcessor::storePacket(short *packet, int packet_length)
{
    int free = buffer_size - (counter1 + 1);
    //освободить место для пришедшего пакета
    if (packet_length > free) {
        int shift = packet_length - free;
        memmove(processorBuffer, processorBuffer + shift, (buffer_size - shift) * sizeof(short));
        counter1 -= shift;
        //не даём counter2 стать отрицательным, так мы теряем данные, но это так и должно быть
        counter2 = counter2 <= shift ? 0 : counter2 - shift;
    }
    //скопировать пакет в буфер
    memcpy(processorBuffer + counter1 + 1, packet, packet_length * sizeof(short));
    counter1 += packet_length;
}

void DataProcessor::sendDataToScreen(/*int amount*/)
{
    int rest = buffer_size - counter2;
    if (rest < 0) {
        rest = 0;
    }
    int samplesToSend = std::min(rest, /*amount*/64);
    emit(sendSamples(&processorBuffer[counter2], samplesToSend));
    counter2 += samplesToSend;
}

void DataProcessor::startBuffTimer()
{
    buffTimer->start(10);
}

void DataProcessor::stopBuffTimer()
{
    buffTimer->stop();
}
