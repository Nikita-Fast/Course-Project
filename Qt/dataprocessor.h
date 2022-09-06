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

  bool trigger_enabled = false;
  bool paused_after_trigger = false;
  bool continuous_trigger_enabled = true;
  short trigger_level = 5000;

  StrictRingBuffer* samples_delay_line;
  StrictRingBuffer* before_trigger_buffer;



  bool collecting_is_started = false;
  int samples_collected = 0;
  short trigger_buf[4000];
  int trigger_offset_in_samples = 0;

 public slots:
  void set_is_paused_true();  // TODO: это ай яй яй
  void set_is_paused_false();

  void writePacketToBuf(short* packet, int length);

  void toggle_trigger();
  void set_trigger_level(int);
  void change_trigger_mode(int);
  void set_trigger_offset(int);

 private:

  bool is_paused = false;
  static const int input_freq = 64000;
  const int oscill_freq;
  StrictRingBuffer* buffer;
  void apply_trigger(short* samples, int samples_num);
  bool is_triggered(short sample);
  void apply_continuous_trigger(short* samples, int samples_num);

};

#endif  // DATAPROCESSOR_H
