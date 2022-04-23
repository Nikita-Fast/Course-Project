#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
//#include <array> // TODO: лишний инклюд
#include "StrictRingBuffer.h"
#include "nonmoveable.h"

//#include "ringbuffer.h"

// TODO: экран работает на своей частоте дискретизации (обычно где нибудь 50
// МГц). Входные данные должны поступать на него с этой частотой
class Screen final : public QWidget, private NonMoveable<Screen> {
  Q_OBJECT
 public:
  explicit Screen(QWidget* parent = nullptr);
  ~Screen();

  void set_buffer(StrictRingBuffer* buffer);

 public slots:

  void
  increaseScaleY();  // TODO: зачем нам изменение масштаба шагами? Почему нельзя
                     // установить тот масштаб, который нужно? Чему равен шаг?
  void decreaseScaleY();
  void increaseScaleX();
  void decreaseScaleX();

  void shiftToLeft();  // TODO: на сколько сдвиг?
  void shiftToRight();
  void shiftUp();
  void shiftDown();

 protected:
  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent* event);

 private:
  std::vector<QLine> v_lines;
  std::vector<QLine> h_lines;
  QLine* zero_level_line = nullptr;
  int top = 0;
  int bottom = 0;
  static const int RANGE = 65536 * 2;

  StrictRingBuffer* buffer;

  void create_grid();
  void update_top_bottom();
  void drawGrid();
  void convertBufferToPoints();

  //  RingBuffer<short, screen_buffer_size> ring_buffer;  // TODO: почему short?
  QPoint* points;
  QTimer* screen_timer;

  int y_scale = 2;
  int x_scale = 2;
  // int pivot_y = height_pixels / 2;
  int pivot_y = 0;
  int rendered_part_start = 0;  //начало отрисовываемой части буфера

  const int grid_horizontal_segments = 7;
  const int grid_vertical_segments = 5;

  bool is_paused = false;
};

#endif  // SCREEN_H
