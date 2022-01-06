#include "dataprocessor.h"
#include "QtDebug"
#include <QTimer>
#include "StrictRingBuffer.h"
#include "worker.h"

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
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
    delete buffer;
}


void DataProcessor::writePacketToBuf(short *packet, int length)
{
    buffer->write(packet, length);

    tryProcessFrame();
}

void DataProcessor::receiveProcessedFrame(short *frame)
{
    frameIsCurrentlyProcessing = false;
    tryProcessFrame();

    emit(sendFrameToScreen(frame, frame_size));
}

bool DataProcessor::frameIsReady()
{
    if (buffer->getElementsCount() >= frame_size) {
        return true;
    }
    return false;
}

void DataProcessor::tryProcessFrame()
{
    if (frameIsReady() && !frameIsCurrentlyProcessing) {
        frameIsCurrentlyProcessing = true;
        emit(frameReadyForProcessing(buffer));
    }
}
