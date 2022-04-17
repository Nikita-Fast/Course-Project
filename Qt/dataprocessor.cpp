#include "dataprocessor.h"
//#include <QTimer>
#include "QtDebug"
#include "StrictRingBuffer.h"
//#include "worker.h"

DataProcessor::DataProcessor(QObject* parent) : QObject(parent) {
  //  buffer = new StrictRingBuffer(buffer_size);

  //  Worker* worker = new Worker(frame_size);
  //  worker->moveToThread(&worker_thread);

  //  connect(this, SIGNAL(frameReadyForProcessing(StrictRingBuffer*)), worker,
  //          SLOT(processFrame(StrictRingBuffer*)));
  //  connect(worker, SIGNAL(frameProcessed(short*)), this,
  //          SLOT(receiveProcessedFrame(short*)));
  //  connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

  //  worker_thread.start();
}

// DataProcessor::~DataProcessor() {
//  delete buffer;
//}

void DataProcessor::setBuffer(StrictRingBuffer* buffer) {
  this->buffer = buffer;
}

void DataProcessor::writePacketToBuf(short* packet, int length) {
  if (input_freq < oscill_freq) {
    int factor = oscill_freq / input_freq;
    interpolate(packet, length, factor);
  } else {
    int factor = input_freq / oscill_freq;
    decimate(packet, length, factor);
  }

  //  if (frameIsReady()) {
  //    short* frame = new short[frame_size];
  //    buffer->read(frame, frame_size);
  //    emit(sendFrameToScreen(frame, frame_size));
  //  }
  //  tryProcessFrame();
}

void DataProcessor::decimate(short* base, int length, int factor) {
  if (decimation_offset > 0) {
    for (int i = factor - decimation_offset; i < length; i += factor) {
      //      qDebug() << base[i];
      buffer->write_or_rewrite(base[i]);
    }
    decimation_offset = (decimation_offset + length) % factor;
    //    qDebug() << "<><><><><><><><><><><><>";
    return;
  }
  if (decimation_offset == 0) {
    for (int i = 0; i < length; i += factor) {
      //      qDebug() << base[i];
      buffer->write_or_rewrite(base[i]);
    }
    decimation_offset = length % factor;
    //    qDebug() << "<><><><><><><><><><><><>";
    return;
  }
}

void DataProcessor::interpolate(short* base, int length, int factor) {
  for (int i = 0; i < length; i++) {
    short node = base[i];
    int diff = node - last_interpolation_node;
    double step = diff / factor;
    for (int j = 0; j < factor; j++) {
      short i_node = last_interpolation_node + j * step;

      buffer->write_or_rewrite(i_node);
    }
    last_interpolation_node = node;
  }
}

// void DataProcessor::receiveProcessedFrame(short* frame) {
//  frameIsCurrentlyProcessing = false;
//  tryProcessFrame();

//  emit(sendFrameToScreen(frame, frame_size));
//}

// bool DataProcessor::frameIsReady() {
//  return buffer->getElementsCount() >= frame_size;
//}

// void DataProcessor::tryProcessFrame() {
//  if (frameIsReady() && !frameIsCurrentlyProcessing) {
//    frameIsCurrentlyProcessing = true;
//    emit(frameReadyForProcessing(buffer));
//  }
//}
