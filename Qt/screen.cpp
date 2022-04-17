#include "screen.h"
#include <QTimer>
#include "QtDebug"
#include "cmath"
#include "qpainter.h"

Screen::Screen(QWidget* parent) : QWidget(parent) {
  // TODO: экран должен менять свои размеры. Можно задаться минимальным размером
  // (если точек на экране (пикселей) меньше чем нужно отобразить, то в этом
  // случае нужно делать децимацию)
  setFixedSize(width_pixels, height_pixels);

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::black);
  setAutoFillBackground(true);
  setPalette(pal);

  points = new QPoint[width()];  // TODO:Почему не stl?
  screen_timer = new QTimer(this);
  connect(screen_timer, SIGNAL(timeout()), this,
          SLOT(update()));  // TODO: bool при ретурне не контролируется
  screen_timer->start(16);  // TODO: бага. Ты запускаешь таймер раньше чем
                            // выделяешь память под points
}

Screen::~Screen() {
  delete points;
  delete screen_timer;  // TODO: бага
}

void Screen::set_buffer(StrictRingBuffer* buffer) {
  this->buffer = buffer;
}

void Screen::increaseScaleY() {
  // TODO: В моем представлении scale должен быть целым числом.
  // положительные увеличивают кратно, отрицательные уменьшают кратно
  if (y_scale < 32768) {
    y_scale *= 2;
  }
}

void Screen::decreaseScaleY() {
  if (y_scale > 1) {
    y_scale /= 2;
  }
}

void Screen::increaseScaleX() {
  if (x_scale > 1) {
    x_scale--;
  }
}

void Screen::decreaseScaleX() {
  if ((x_scale + 1) * width() <= buffer->get_capacity()) {
    x_scale++;

    int len = rendered_part_start + width() * x_scale;
    int excess = len - buffer->get_capacity();
    if (excess > 0) {
      rendered_part_start -= excess;
    }
  }
  //  qDebug() << x_scale;
}

void Screen::shiftToLeft() {
  for (int i = 0; i < 100; i++) {
    if (rendered_part_start > 0) {
      rendered_part_start--;
    }
  }
}

void Screen::shiftToRight() {
  for (int i = 0; i < 100; i++) {
    if (rendered_part_start + 1 + width() * x_scale <= buffer->get_capacity()) {
      rendered_part_start++;
    }
  }
  qDebug() << rendered_part_start;
}

void Screen::shiftUp() {
  pivot_y += 25;
}

void Screen::shiftDown() {
  pivot_y -= 25;
}

void Screen::paintEvent(QPaintEvent*) {
  convertBufferToPoints();

  drawGrid();
  QPainter painter(this);

  QPen pointPen(Qt::red, 4);
  painter.setPen(pointPen);

  painter.drawPolyline(points, width());
}

void Screen::convertBufferToPoints() {
  // TODO:Почему мы рассчитываем масштаб во время отрисовки. Почему мы не можем
  // это делать при укладке данных в буфер экрана?
  //при текущем подходе большая вычислительная нагрузка получается

  //страшно ли переполнение y?; при большом масштабе по у scale_y стремится
  //к нулю, y -> бесконечности
  // TODO: конечно могут быть переполнения. У тебя scale должен быть
  // ограничен (очевидно, что если у тебя размах по y превышает шаг
  // квантования, то нет смысла увеличивать)
  int tail = buffer->get_capacity() - rendered_part_start - width() * x_scale;
  int fst_projected = (buffer->get_w_index() - tail - width() * x_scale) %
                      buffer->get_capacity();
  if (fst_projected < 0) {
    fst_projected = buffer->get_capacity() + fst_projected;
  }
  double pixel_y_size = 65536.0 / (height() * y_scale);
  for (int i = 0; i < width(); i++) {
    int y_pos =
        pivot_y - buffer->peekAt(fst_projected + i * x_scale) / pixel_y_size;
    points[i] = QPoint(i, y_pos);
  }
}

void Screen::drawGrid() {
  // TODO: зачем нам тут рассчитывать масштабы?

  QPainter painter(this);
  QPen gridPen(Qt::gray, 0, Qt::DotLine);
  painter.setPen(gridPen);

  int size = grid_horizontal_segments - 1;
  QLine vLines[size];  // TODO: массив в стеке в не хорошо. Функция критична к
                       // времени выполнения.
  int hStep = width() / grid_horizontal_segments;

  for (int i = 0; i < size; i++) {
    vLines[i] = QLine(hStep * (i + 1), 0, hStep * (i + 1), height() - 1);
  }
  painter.drawLines(vLines, size);

  int size2 = grid_vertical_segments - 1;
  QLine hLines[size2];
  int vStep = height() / grid_vertical_segments;

  for (int i = 0; i < size2; i++) {
    hLines[i] = QLine(0, vStep * (i + 1), width() - 1, vStep * (i + 1));
  }
  painter.drawLines(hLines, size2);
}
