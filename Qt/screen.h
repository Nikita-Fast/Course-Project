#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
//#include <array> // TODO: лишний инклюд
#include "nonmoveable.h"
#include "ringbuffer.h"

// TODO: экран работает на своей частоте дискретизации (обычно где нибудь 50 МГц). Входные данные должны поступать на него с этой частотой
class Screen final : public QWidget, private NonMoveable<Screen>
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = nullptr);
    ~Screen();

signals:
    void yScaleChanged(QString);// TODO: на мой взгляд сигнатура неудачна. Зачем этому сигналу передавать строку?
    void xScaleChanged(QString);

public slots:
    void updateScreen(); // TODO: Зачем отдельный метод?

    void increaseScaleY(); // TODO: зачем нам изменение масштаба шагами? Почему нельзя установить тот масштаб, который нужно? Чему равен шаг?
    void decreaseScaleY();
    void increaseScaleX();
    void decreaseScaleX();

    void shiftToLeft();// TODO: на сколько сдвиг?
    void shiftToRight();
    void shiftUp();
    void shiftDown();

    void receiveFrame(short *, int);// TODO: экран ничего не принимает. Он отображает данные в буфере. Наверное лучше сделать метод инкапсуляции в буфер. Поему short? Нужно ли нам делать размер пакета произвольной длины?
    void setIsPausedToTrue(); // TODO: это ай яй яй
    void setIsPausedToFalse();

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawGrid();
    void convertBufferToPoints(QPoint *points);

    enum {
        screen_buffer_size = 1500,// TODO: что это? и почему оно 1500?
        width_pixels = 700, // TODO: это не используется
        height_pixels = 500 // TODO: что это? и почему оно 500?
    };

    RingBuffer<short, screen_buffer_size> ring_buffer; // TODO: почему short?
    QPoint *points;
    QTimer *screenTimer;

    // TODO: этот параметр используется только в одном месте. Зачем делать его членом класса (да еще и не константным)
    const int screenTimerPeriod = 16;

    int samplingRate = 64000; // TODO: поле класса записано стилистически как метод. Так не делают.
    double sample_size_ms = 1000.0 / samplingRate; // TODO: это период дискретизации.

    double scale_y = 150; // TODO: почему это double?
    int pivot_y = height_pixels / 2;
    int rendered_part_samples_length = screen_buffer_size / 2;
    int rendered_part_start = screen_buffer_size / 4; //начало отрисовываемой части буфера

    const double scaling_factor_y = 1.25; // TODO: почему это double? и почему 1,25?
    const double scaling_factor_x = 1.25;
    const int grid_horizontal_segments = 7;
    const int grid_vertical_segments = 5;
    const int vertical_shift_magnitude = height_pixels / 10;

    bool isPaused = false;



};

#endif // SCREEN_H
