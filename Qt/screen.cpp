#include "screen.h"
#include <QTimer>
#include "qpainter.h"
#include "QtDebug"
#include "cmath"
#include "ringbuffer.h"// TODO: лишний инклюд
#include <QThread>


Screen::Screen(QWidget *parent) : QWidget(parent)
{
    // TODO: экран должен менять свои размеры. Можно задаться минимальным размером (если точек на экране (пикселей) меньше чем нужно отобразить,
    // то в этом случае нужно делать децимацию)
    //setFixedSize(width_pixels, height_pixels);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    screenTimer = new QTimer(this);
    connect(screenTimer, SIGNAL(timeout()), this, SLOT(updateScreen()));
    screenTimer->start(screenTimerPeriod);

    ring_buffer.setOffset(rendered_part_start);

    points = new QPoint[width()];
}

Screen::~Screen()
{
    delete points;
    delete screenTimer;
}

void Screen::receiveFrame(short *frame, int frame_size)
{
    if (!isPaused) {
        for (int i = 0; i < frame_size; i++) {
            ring_buffer.put(*(frame + i));
        }
    }
    delete frame;
}

void Screen::setIsPausedToTrue()
{
    isPaused = true;
}

void Screen::setIsPausedToFalse()
{
    isPaused = false;
}

void Screen::updateScreen()
{
    update();
}

void Screen::increaseScaleY()
{
    scale_y /= scaling_factor_y;
    emit(yScaleChanged(QString("y = ") + QString::number(scale_y * (height_pixels / grid_vertical_segments))));
}

void Screen::decreaseScaleY()
{
    scale_y *= scaling_factor_y;
    emit(yScaleChanged(QString("y = ") + QString::number(scale_y * (height_pixels / grid_vertical_segments))));
}

void Screen::increaseScaleX()
{
    int decrease = rendered_part_samples_length - (rendered_part_samples_length / scaling_factor_x);
    int left = decrease / 2;
    rendered_part_start += left;
    rendered_part_samples_length -= decrease;
    ring_buffer.setOffset(rendered_part_start);
    emit(xScaleChanged(QString("x = ") +
                       QString::number(rendered_part_samples_length / grid_horizontal_segments * sample_size_ms) + QString(" ms")));
}

void Screen::decreaseScaleX()
{
    int increase = rendered_part_samples_length * scaling_factor_x - rendered_part_samples_length;
    int left = increase / 2;

    rendered_part_start -= left;
    rendered_part_samples_length += increase;

    if (rendered_part_start < 0) {
        int shift = 0 - rendered_part_start;
        rendered_part_start = 0;
        rendered_part_samples_length += shift;

        if (rendered_part_samples_length > screen_buffer_size) {
            rendered_part_samples_length = screen_buffer_size;
        }
    }

    if (rendered_part_start + rendered_part_samples_length > screen_buffer_size) {
        int shift = rendered_part_start + rendered_part_samples_length - screen_buffer_size;
        if (rendered_part_start >= shift) {
            rendered_part_start -= shift;
        }
        else {
            shift -= rendered_part_start;
            rendered_part_start = 0;
            rendered_part_samples_length -= shift;
        }
    }
    ring_buffer.setOffset(rendered_part_start);

    emit(xScaleChanged(QString("x = ") +
                       QString::number(rendered_part_samples_length / grid_horizontal_segments * sample_size_ms) + QString(" ms")));
}

void Screen::shiftToLeft()
{
    int shift = rendered_part_samples_length / 10;
    rendered_part_start = rendered_part_start <= shift ? 0 : rendered_part_start - shift;
    ring_buffer.setOffset(rendered_part_start);
}

void Screen::shiftToRight()
{
    int shift = rendered_part_samples_length / 10;
    int offset = screen_buffer_size - (rendered_part_start + rendered_part_samples_length);
    rendered_part_start += std::min(offset, shift);
    ring_buffer.setOffset(rendered_part_start);
}

void Screen::shiftUp()
{
    pivot_y += vertical_shift_magnitude;
}

void Screen::shiftDown()
{
    pivot_y -= vertical_shift_magnitude;
}

void Screen::paintEvent(QPaintEvent *)
{
    int pointsNumber = std::min(width(), rendered_part_samples_length);

    convertBufferToPoints(points);

    drawGrid();
    QPainter painter(this);

    QPen pointPen(Qt::red, 4);
    painter.setPen(pointPen);

    painter.drawPolyline(points, pointsNumber);
}

void Screen::convertBufferToPoints(QPoint *points)
{
    if (width() >= rendered_part_samples_length) {
        double step = width() / (double)rendered_part_samples_length;
        for (int i = 0; i < rendered_part_samples_length; i++) {
            int x = round(i * step);
            //страшно ли переполнение y?; при большом масштабе по у scale_y стремится к нулю, y -> бесконечности
            // TODO: конечно могут быть переполнения. У тебя scale должен быть ограничен (очевидно, что если у тебя размах по y превышает шаг квантования, то нет смысла увеличивать)

            int y = pivot_y - ring_buffer.get(i) / scale_y;

            points[i] = QPoint(x, y);
        }
    }
    else {
        double step = (double)rendered_part_samples_length / width();
        for (int i = 0; i < width(); i++) {
            int x = i;
            int y = pivot_y - ring_buffer.get(floor(i * step)) / scale_y;

            points[i] = QPoint(x, y);
        }
    }
}

void Screen::drawGrid()
{
    QPainter painter(this);
    QPen gridPen(Qt::gray, 0, Qt::DotLine);
    painter.setPen(gridPen);

    int size = grid_horizontal_segments - 1;
    QLine vLines[size];
    int hStep = width() / grid_horizontal_segments;

    for (int i = 0; i < size; i++) {
        vLines[i] = QLine(hStep * (i + 1), 0, hStep * (i + 1), height() - 1);
    }
    painter.drawLines(vLines, size);

    int size2 = grid_vertical_segments - 1;
    QLine hLines[size2];
    int vStep = height() / grid_vertical_segments;

    for (int i = 0; i < size2; i++) {
        hLines[i] = QLine(0, vStep * (i + 1), width() - 1,  vStep * (i + 1));
    }
    painter.drawLines(hLines, size2);
}
