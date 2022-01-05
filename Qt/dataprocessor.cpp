#include "dataprocessor.h"
#include "QtDebug"
#include <QTimer>
#include "StrictRingBuffer.h"
#include "worker.h"

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
//    buffTimer = new QTimer;
//    connect(buffTimer, SIGNAL(timeout()), this, SLOT(sendDataToScreen()));
    buffer = new StrictRingBuffer(buffer_size);

    Worker *worker = new Worker(frame_size);
    worker->moveToThread(&workerThread);

    connect(this, SIGNAL(frameReadyForProcessing(StrictRingBuffer *)), worker, SLOT(processFrame(StrictRingBuffer *)));
    connect(worker, SIGNAL(frameProcessed(short *)), this, SLOT(receiveProcessedFrame(short *)));
    connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    workerThread.start();
}

DataProcessor::~DataProcessor()
{
    //delete buffTimer;
}


void DataProcessor::writePacketToBuf(short *packet, int length)
{
    //qDebug() << QThread::currentThread() << " writePacketToBuf()";
    buffer->write(packet, length);
    qDebug() << buffer->getElementsCount();

    if (frameIsReady() && !frameIsCurrentlyProcessing) {
        frameIsCurrentlyProcessing = true;
        //qDebug() << "frame ready";
        emit(frameReadyForProcessing(buffer));
    }
}

void DataProcessor::receiveProcessedFrame(short *frame)
{

    frameIsCurrentlyProcessing = false;
    //qDebug() << QThread::currentThread() << " DataProcessor::receiveProcessedFrame";
    //заход на обработку следующего фрейма, по идее здесь queued connection и ждать срабатывания слота в worker не надо
    //поэтому тратится времени немного
    if (frameIsReady() && !frameIsCurrentlyProcessing) {
        frameIsCurrentlyProcessing = true;
        emit(frameReadyForProcessing(buffer));
    }

    emit(sendFrameToScreen(frame, frame_size));
}

bool DataProcessor::frameIsReady()
{
    if (buffer->getElementsCount() >= frame_size) {
        return true;
    }
    return false;
}
