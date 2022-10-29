#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QMainWindow>
#include "nonmoveable.h"

class DataProcessor;
class Screen;
class DataInterface;
class StrictRingBuffer;
class MyWindow;
class QLineEdit;
class QComboBox;

class Oscilloscope final : public QMainWindow,
                           private NonMoveable<Oscilloscope> {
  Q_OBJECT

 public:
  explicit Oscilloscope(
            QWidget* parent = nullptr
            );
  ~Oscilloscope();
  static const int BUFFER_SIZE = 4000;
  static const int OSCILL_FREQ_HZ = 64000;

 signals:
  void trigger_lvl_updated(int);
  void trigger_offset_updated(int);

public slots:
  void update_max_width(int curr_max_screen_width);
  void read_trigger_level();
  void read_trigger_offset();

protected:
 void changeEvent(QEvent *event);

 private:

  Screen* screen;
  DataProcessor* processor;
  DataInterface* dataInterface;
  StrictRingBuffer* buffer;

  QLineEdit* trigger_level;
  QComboBox* trigger_mode;
  QLineEdit* trigger_offset;
  int trigger_offset_in_samples = 0;
};
#endif  // OSCILLOSCOPE_H
