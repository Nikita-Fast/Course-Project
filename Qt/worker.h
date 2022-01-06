#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class StrictRingBuffer;

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(int frame_size);

signals:
    void frameProcessed(short *);

public slots:
    void processFrame(StrictRingBuffer *buf);

private:
    int frame_size;

    void processingFunction1(short *frame);
};

#endif // WORKER_H
