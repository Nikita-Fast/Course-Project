#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QMainWindow>
#include "nonmoveable.h"

class DataProcessor;
class Screen;
class DataInterface;
class StrictRingBuffer;

class Oscilloscope final : public QMainWindow,
                           private NonMoveable<Oscilloscope> {
  Q_OBJECT

 public:
  explicit Oscilloscope(QWidget* parent = nullptr);
  ~Oscilloscope();
  static const int BUFFER_SIZE = 4000;
  static const int OSCILL_FREQ_HZ = 8000;

 private:
  Screen* screen;
  DataProcessor* processor;
  DataInterface* dataInterface;
  StrictRingBuffer* buffer;
};
#endif  // OSCILLOSCOPE_H
