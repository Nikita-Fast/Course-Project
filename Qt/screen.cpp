#include "screen.h"
#include <QHBoxLayout>
#include <QTimer>
#include "QtDebug"
#include "cmath"
#include "qpainter.h"
#include <QResizeEvent>

Screen::Screen(int oscill_freq, QWidget* parent) : QWidget(parent), oscill_freq(oscill_freq) {
  // TODO: экран должен менять свои размеры. Можно задаться минимальным размером
  // (если точек на экране (пикселей) меньше чем нужно отобразить, то в этом
  // случае нужно делать децимацию)
  setMinimumSize(700, 500);
  setMaximumSize(1600, 1000);

  top = 32768;
  bottom = -32768;
  // setFixedSize(width_pixels, height_pixels);

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::black);
  setAutoFillBackground(true);
  setPalette(pal);

  create_grid();

  int w = maximumWidth();
  points = new QPoint[w];  // TODO:Почему не stl?
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

int Screen::get_rendered_part_start()
{
    return rendered_part_start;
}

int Screen::get_x_scale()
{
    return x_scale;
}

void Screen::increaseScaleY() {
  // TODO: В моем представлении scale должен быть целым числом.
  // положительные увеличивают кратно, отрицательные уменьшают кратно
  if (y_scale < 32768) {
    y_scale *= 2;
    update_top_bottom();
    create_grid();
  }
}

void Screen::decreaseScaleY() {
  if (y_scale > 1) {
    y_scale /= 2;
    update_top_bottom();
    create_grid();
  }
}

void Screen::increaseScaleX() {
  if (x_scale > 1) {
    x_scale--;
    create_grid();
  }
}

void Screen::decreaseScaleX() {
  if ((x_scale + 1) * maximumWidth() <= buffer->get_capacity()) {
    x_scale++;

    int len = rendered_part_start + width() * x_scale;
    int excess = len - buffer->get_capacity();
    if (excess > 0) {
      rendered_part_start -= excess;
    }
    create_grid();
  }
}

void Screen::shiftToLeft() {
  for (int i = 0; i < 100; i++) {
    if (rendered_part_start >= x_scale) {
      rendered_part_start -= x_scale;
    }
  }
  create_grid();
}

void Screen::shiftToRight() {
  for (int i = 0; i < 100; i++) {
    if (rendered_part_start + x_scale + width() * x_scale <=
        buffer->get_capacity()) {
      rendered_part_start += x_scale;
    }
  }
  create_grid();
  //  qDebug() << rendered_part_start;
}

void Screen::shiftUp() {
  // pivot_y += 25;
  int diff = (RANGE / y_scale) / 10;
  top += diff;
  bottom += diff;
  create_grid();
}

void Screen::shiftDown() {
  // pivot_y -= 25;
  int diff = (RANGE / y_scale) / 10;
  top -= diff;
  bottom -= diff;
  create_grid();
}

void Screen::paintEvent(QPaintEvent*) {
  convertBufferToPoints();

  drawGrid();
  QPainter painter(this);

  QPen pointPen(Qt::red, 4);
  painter.setPen(pointPen);

  painter.drawPolyline(points, width());
  //  qDebug() << top << ", " << bottom;
  //  qDebug() << h_grid_step_us << ", " << v_grid_step;
  //  qDebug() << rendered_part_start;
}

void Screen::update_top_bottom() {
  int max_screen_y_range = RANGE / y_scale;
  double part = height() / (double)maximumHeight();
  int cur_screen_y_range = max_screen_y_range * part;
  int old = top - bottom;
  int diff = cur_screen_y_range - old;
  int inc_up = diff / 2;
  top += inc_up;
  bottom -= diff - inc_up;
}

void Screen::resizeEvent(QResizeEvent* event) {
  update_top_bottom();
  create_grid();

//  const QSize sizeEventOld = event->oldSize();
//  const QSize sizeEvent = event->size();
//  qDebug() << sizeEventOld << ", " << sizeEvent;

//  if (rendered_part_start + sizeEvent.width() * x_scale > buffer->get_capacity()) {
//      qDebug() << "went out of buffer";
//        resize(sizeEventOld);
//  }
//  else {
//      QWidget::resizeEvent(event);
//  }
//  QWidget::resizeEvent(event);
}

void Screen::choose_v_grid_step() {
  double pixel_y_size = (top - bottom) / (double)height();
  int v_step = v_grid_step / pixel_y_size;
  if (maximumHeight() / v_step > 8) {
    v_grid_step *= 2;
  } else {
    if (maximumHeight() / v_step < 4) {
      v_grid_step /= 2;
    }
  }
  emit(send_v_grid_step(v_grid_step));
}

void Screen::choose_h_grid_step() {

    double discretization_period_us = 1000000.0 / oscill_freq;
    double full_screen_time_us = maximumWidth() * x_scale * discretization_period_us;

    int k = full_screen_time_us / h_grid_step_us;
    if (k > 8 || k < 4) {
        h_grid_step_us = floor( full_screen_time_us / 6);
    }

  emit(send_h_grid_step(h_grid_step_us));
}

void Screen::create_grid() {
  h_lines.clear();
  v_lines.clear();

  choose_h_grid_step();
  int h_step = (h_grid_step_us  * (oscill_freq / 1000000.0)) / x_scale;
  int max = 0;

  /*подозреваю, важно, что сейчас частота экрана 1 МГц и поэтому
     rendered_part_start, который обозначает число сэплов, на которые мы
     отступили вправо, совпадает с числом микросекунд, которым равен этот
     отступ. При другой частоте получение x_base вероятно изменится*/
  int h_grid_step_in_samples = (h_grid_step_us  * (oscill_freq / 1000000.0));

  int x_base = (rendered_part_start / (h_grid_step_us  * h_grid_step_in_samples) + 1) * h_grid_step_in_samples;

  for (int x = (x_base - rendered_part_start) / x_scale, j = 0;
       x < maximumWidth(); x += h_step, j++) {
    v_lines.push_back(QLine(x, 0, x, maximumHeight() - 1));

    if (j < x_labels.size()) {
      x_labels.at(j)->setNum((x_base / ((oscill_freq / 1000000.0))) + h_grid_step_us * j);
      x_labels.at(j)->setGeometry(x + 1, 30, 50, 20);
      x_labels.at(j)->setVisible(true);
    } else {
      QLabel* lbl = new QLabel(this);
      lbl->setNum(h_grid_step_us * j);
      lbl->setGeometry(x, 30, 50, 20);
      lbl->setStyleSheet(
          "QLabel { background-color : rgba(0,0,0,0%); color : yellow}");
      x_labels.push_back(lbl);
    }
    max = j;
  }

  for (int i = max + 1; i < x_labels.size(); i++) {
    x_labels.at(i)->setVisible(false);
  }
  max = 0;

  choose_v_grid_step();
  int v_dist = v_grid_step;
  double pixel_y_size = (top - bottom) / (double)height();
  int y_base = ceil(bottom / (double)v_dist) * v_dist;
  for (int y = y_base, j = 0; y < top; y += v_dist, j++) {
    int y_pos = height() - ((y - bottom) / pixel_y_size);
    h_lines.push_back(QLine(0, y_pos, maximumWidth() - 1, y_pos));

    if (j < y_labels.size()) {
      y_labels.at(j)->setNum(y);
      y_labels.at(j)->setGeometry(20, y_pos + 1, 50, 20);
      y_labels.at(j)->setVisible(true);
    } else {
      QLabel* lbl = new QLabel(this);
      lbl->setNum(h_grid_step_us * j);
      lbl->setGeometry(20, y_pos, 50, 20);
      lbl->setStyleSheet(
          "QLabel { background-color : rgba(0,0,0,0%); color : cyan}");
      y_labels.push_back(lbl);
    }
    max = j;
  }
  for (int i = max + 1; i < y_labels.size(); i++) {
    y_labels.at(i)->setVisible(false);
  }

  if (top > 0 && bottom < 0) {
    int y = height() - ((0 - bottom) / pixel_y_size);
    zero_level_line = new QLine(0, y, maximumWidth() - 1, y);
  } else {
    if (zero_level_line != nullptr) {
      free(zero_level_line);
    }
    zero_level_line = nullptr;
  }
}

void Screen::convertBufferToPoints() {
  // TODO:Почему мы рассчитываем масштаб во время отрисовки. Почему мы не можем
  // это делать при укладке данных в буфер экрана?
  //при текущем подходе большая вычислительная нагрузка получается

  int tail = buffer->get_capacity() - rendered_part_start - width() * x_scale;
  int fst_projected = (buffer->get_w_index() - tail - width() * x_scale) %
                      buffer->get_capacity();
  if (fst_projected < 0) {
    fst_projected = buffer->get_capacity() + fst_projected;
  }
  double pixel_y_size = RANGE / (double)(maximumHeight() * y_scale);

  for (int i = 0; i < width(); i++) {
    int sample = buffer->peekAt(fst_projected + i * x_scale);
    int y = (sample - bottom) / pixel_y_size;
    int point_y_pos = height() - y;

    points[i] = QPoint(i, point_y_pos);
  }
}

void Screen::drawGrid() {
  QPainter painter(this);
  QPen gridPen(Qt::gray, 0, Qt::DotLine);
  painter.setPen(gridPen);

  painter.drawLines(v_lines.data(), v_lines.size());
  painter.drawLines(h_lines.data(), h_lines.size());

  if (zero_level_line != nullptr) {
    QPen zero_level_line_pen(Qt::gray, 2, Qt::SolidLine);
    painter.setPen(zero_level_line_pen);
    painter.drawLine(*zero_level_line);
  }
}
