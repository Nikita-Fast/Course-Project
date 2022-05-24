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
  static const int input_freq = 1000000;
  const int oscill_freq;

  //  значение в последнем узле интерполяции в предыдущей итерации
  short last_interpolation_node = 0;
  short decimation_offset = 0;

  StrictRingBuffer* buffer;

  void interpolate(short* base, int length, int factor);
  void decimate(short* base, int length, int factor);
};

#endif  // DATAPROCESSOR_H
