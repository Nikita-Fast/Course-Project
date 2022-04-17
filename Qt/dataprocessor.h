#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QThread>

class QTimer;
class StrictRingBuffer;

// TODO: класс должен принимать данные из интерфейса, поступающие на частоте
// дискретизации А и передискретизировать их в частоту экрана В
class DataProcessor : public QObject {
  Q_OBJECT
 public:
  explicit DataProcessor(QObject* parent = nullptr);
  ~DataProcessor();

 public slots:

  void writePacketToBuf(short* packet, int length);
  void receiveProcessedFrame(short* frame);

 signals:
  void frameReadyForProcessing(StrictRingBuffer* buffer);
  void sendFrameToScreen(short* frame, int frame_size);

 private:
  static const int input_freq = 25000;
  static const int oscill_freq = 25000 * 4;
  static const int buffer_size = 16384;
  static const int frame_size = 512 * 4;

  //  значение в последнем узле интерполяции в предыдущей итерации
  short last_interpolation_node = 0;
  short decimation_offset = 0;

  StrictRingBuffer* buffer;
  QThread worker_thread;

  bool frameIsReady();
  bool frameIsCurrentlyProcessing = false;
  void tryProcessFrame();

  /* эти методы пишут в буфер */
  void interpolate(short* base, int length, int factor);
  void decimate(short* base, int length, int factor);
};

#endif  // DATAPROCESSOR_H
