#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include "nonmoveable.h"
#include <array>

class Screen final : public QWidget, private NonMoveable<Screen>
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = nullptr);

signals:
    void getNewValues(int amount);
    void yScaleChanged(QString);
    void xScaleChanged(QString);

public slots:
    void updateScreen();
    void updateScreenBuffer(short*, int);
    void askForData();

    void increaseScaleY();
    void decreaseScaleY();
    void increaseScaleX();
    void decreaseScaleX();

    void shiftToLeft();
    void shiftToRight();
    void shiftUp();
    void shiftDown();

    void startBufferTimer();
    void stopBufferTimer();

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawGrid();
    void convertBufferToPoints(QPoint *points);

    enum {
        screen_buffer_size = 6400,
        width_pixels = 700,
        height_pixels = 500
    };

    std::array<short, screen_buffer_size> screenBuffer;
    //QTimer *bufferTimer;


    // TODO: этот параметр используется только в одном месте. Зачем делать его членом класса (да еще и не константным)
    int screenTimerPeriod = 16;
    int bufferTimerPeriod = 10;
    int samplingRate = 6400; //делал тест на нагрузку, поэтому тут такое странное значение
    double sample_size_ms = 1000.0 / samplingRate;

    // TODO: стремная инициализация члена класса. А что если мой компилятор сначала его инициализирует, а только затем bufferTimerPeriod
    int valuesToAsk = (samplingRate / 1000.0) * bufferTimerPeriod;

    double scale_y = 150;
    int pivot_y = height_pixels / 2;
    int rendered_part_samples_length = screen_buffer_size / 4;
    int rendered_part_start = screen_buffer_size / 8; //начало отрисовываемой части буфера

    double scaling_factor_y = 1.25;
    double scaling_factor_x = 1.25;
    int grid_horizontal_segments = 7;
    int grid_vertical_segments = 5;
    int vertical_shift_magnitude = height_pixels / 10;


};

#endif // SCREEN_H
