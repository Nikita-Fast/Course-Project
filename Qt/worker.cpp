#include "worker.h"
#include "StrictRingBuffer.h"
#include <QThread>

Worker::Worker(int frame_size)
{
    this->frame_size = frame_size;
}

void Worker::processFrame(StrictRingBuffer *buf)
{
    //qDebug() << QThread::currentThread() << " Worker::processFrame";
    short *frame = new short[frame_size];
    for (int i = 0; i < frame_size; i++) {
        frame[i] = buf->read();
    }

    //processing
    for (int i = 0; i < frame_size; i++) {
        if (frame[i] > 15000) {
            frame[i] = 15000;
        }
    }

    //send to screen
    //кто освобождает память? Пусть пока это будет Screen::receiveFrame
    emit(frameProcessed(frame));
}
