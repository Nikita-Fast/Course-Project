#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include "nonmoveable.h"
#include <array>
#include "ringbuffer.h"

class Screen final : public QWidget, private NonMoveable<Screen>
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = nullptr);

signals:
    void yScaleChanged(QString);
    void xScaleChanged(QString);

public slots:
    void updateScreen();

    void increaseScaleY();
    void decreaseScaleY();
    void increaseScaleX();
    void decreaseScaleX();

    void shiftToLeft();
    void shiftToRight();
    void shiftUp();
    void shiftDown();

    void receiveFrame(short *, int);

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawGrid();
    void convertBufferToPoints(QPoint *points);

    enum {
        screen_buffer_size = 750,
        width_pixels = 700,
        height_pixels = 500
    };

    RingBuffer<short, screen_buffer_size> ring_buffer;

    // TODO: этот параметр используется только в одном месте. Зачем делать его членом класса (да еще и не константным)
    const int screenTimerPeriod = 16;

    int samplingRate = 64000;
    double sample_size_ms = 1000.0 / samplingRate;

    double scale_y = 150;
    int pivot_y = height_pixels / 2;
    int rendered_part_samples_length = screen_buffer_size / 2;
    int rendered_part_start = screen_buffer_size / 4; //начало отрисовываемой части буфера

    double scaling_factor_y = 1.25;
    double scaling_factor_x = 1.25;
    int grid_horizontal_segments = 7;
    int grid_vertical_segments = 5;
    int vertical_shift_magnitude = height_pixels / 10;


};

#endif // SCREEN_H
