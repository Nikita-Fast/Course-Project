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

    void writePacketToBuf(short *packet, int length);
    void receiveProcessedFrame(short *frame);

signals:
    void frameReadyForProcessing(StrictRingBuffer *buffer);
    void sendFrameToScreen(short *frame, int frame_size);

private:
    static const int buffer_size = 16384;
    static const int frame_size = 512;

    StrictRingBuffer *buffer;
    QThread workerThread;

    bool frameIsReady();
    bool frameIsCurrentlyProcessing = false;
    void tryProcessFrame();

};




#endif // DATAPROCESSOR_H
