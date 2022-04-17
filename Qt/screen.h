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

  // Наверное лучше сделать метод инкапсуляции в буфер. Почему short?
  // Нужно ли нам делать размер пакета произвольной длины?

 protected:
  void paintEvent(QPaintEvent*);

 private:
  StrictRingBuffer* buffer;

  void drawGrid();
  void convertBufferToPoints();

  //размеры окна отрисовки в пикселях
  enum {
    width_pixels = 700,  // TODO: это не используется
    height_pixels = 500  // TODO: что это? и почему оно 500?
  };

  //  RingBuffer<short, screen_buffer_size> ring_buffer;  // TODO: почему short?
  QPoint* points;
  QTimer* screen_timer;

  int y_scale = 1;
  int x_scale = 10;
  int pivot_y = height_pixels / 2;
  int rendered_part_start = 0;  //начало отрисовываемой части буфера

  const int grid_horizontal_segments = 7;
  const int grid_vertical_segments = 5;

  bool is_paused = false;
};

#endif  // SCREEN_H
