#include "dataprocessor.h"
#include "QtDebug"
#include "StrictRingBuffer.h"

DataProcessor::DataProcessor(QObject* parent) : QObject(parent) {}

void DataProcessor::setBuffer(StrictRingBuffer* buffer) {
  this->buffer = buffer;
}

void DataProcessor::writePacketToBuf(short* packet, int length) {
  if (!is_paused) {
    if (input_freq < oscill_freq) {
      int factor = oscill_freq / input_freq;
      interpolate(packet, length, factor);
    } else {
      int factor = input_freq / oscill_freq;
      decimate(packet, length, factor);
    }
  }
}

void DataProcessor::decimate(short* base, int length, int factor) {
  if (decimation_offset > 0) {
    for (int i = factor - decimation_offset; i < length; i += factor) {
      buffer->write_or_rewrite(base[i]);
    }
    decimation_offset = (decimation_offset + length) % factor;
    return;
  }
  if (decimation_offset == 0) {
    for (int i = 0; i < length; i += factor) {
      buffer->write_or_rewrite(base[i]);
    }
    decimation_offset = length % factor;
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

void DataProcessor::set_is_paused_true() {
  is_paused = true;
}

void DataProcessor::set_is_paused_false() {
  is_paused = false;
}
