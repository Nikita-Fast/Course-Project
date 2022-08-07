#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QMainWindow>
#include "nonmoveable.h"

class DataProcessor;
class Screen;
class DataInterface;
class StrictRingBuffer;
class MyWindow;

class Oscilloscope final : public QObject,
//        public QMainWindow,
                           private NonMoveable<Oscilloscope> {
  Q_OBJECT

 public:
  explicit Oscilloscope(
            QObject* parent = nullptr
//            QWidget* parent = nullptr
            );
  ~Oscilloscope();
  static const int BUFFER_SIZE = 8000;
  static const int OSCILL_FREQ_HZ = 64000;
  QMainWindow* window;

protected:
// void resizeEvent(QResizeEvent* event);
 bool eventFilter(QObject *obj, QEvent *event) override;

 private:

  Screen* screen;
  DataProcessor* processor;
  DataInterface* dataInterface;
  StrictRingBuffer* buffer;
};
#endif  // OSCILLOSCOPE_H
