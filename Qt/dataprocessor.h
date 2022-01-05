#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QThread>

class QTimer;
class StrictRingBuffer;

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr);
    ~DataProcessor();

public slots:
//    void storePacket(short *packet, int length);
//    void sendDataToScreen(/*int amount*/);

//    void startBuffTimer();
//    void stopBuffTimer();

    void writePacketToBuf(short *packet, int length);
    void receiveProcessedFrame(short *frame);

signals:
    //void sendSamples(short *ptrToValues, int amount);
    /*
по этому сигналу хочу запустить cycle и соответствующий slot должен работать в другом потоке.
слот обработает фрейм, а потом отправляет его в буфер экрана*/
    void frameReadyForProcessing(StrictRingBuffer *buffer);
    void sendFrameToScreen(short *frame, int frame_size);

private:
    static const int buffer_size = 16384;
    static const int frame_size = 512;

    StrictRingBuffer *buffer;
    QThread workerThread;

    bool frameIsReady();
    bool frameIsCurrentlyProcessing = false;

};




#endif // DATAPROCESSOR_H
