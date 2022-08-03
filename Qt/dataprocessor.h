#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>

class StrictRingBuffer;

// TODO: класс должен принимать данные из интерфейса, поступающие на частоте
// дискретизации А и передискретизировать их в частоту экрана В
class DataProcessor : public QObject {
  Q_OBJECT
 public:
  explicit DataProcessor(int oscill_freq,QObject* parent = nullptr);
  void setBuffer(StrictRingBuffer* buffer);

 public slots:
  void set_is_paused_true();  // TODO: это ай яй яй
  void set_is_paused_false();

  void writePacketToBuf(short* packet, int length);

 private:
  bool is_paused = false;
  static const int input_freq = 8000;
  const int oscill_freq;
  StrictRingBuffer* buffer;

};

#endif  // DATAPROCESSOR_H
