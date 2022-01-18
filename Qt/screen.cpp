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
    connect(screenTimer, SIGNAL(timeout()), this, SLOT(updateScreen()));// TODO: bool при ретурне не контролируется
    screenTimer->start(screenTimerPeriod); // TODO: бага. Ты запускаешь таймер раньше чем выделяешь память под points

    ring_buffer.setOffset(rendered_part_start);// TODO: Зачем делать сдвиг пустому буферу. Нужен ли здесь кольцевой буфер?

    points = new QPoint[width()]; // TODO:Почему не stl?
}

Screen::~Screen()
{
    delete points;
    delete screenTimer;// TODO: бага
}

void Screen::receiveFrame(short *frame, int frame_size)
{
    if (!isPaused) {
        for (int i = 0; i < frame_size; i++) {
            ring_buffer.put(*(frame + i));
        }
    }
    delete frame; // TODO: это зло. Ты втихоря стер массив
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
    //TODO: В моем представлении scale должен быть целым числом. положительные увеличивают кратно, отрицательные уменьшают кратно
    scale_y /= scaling_factor_y;
    //TODO: не вижу глубокого смысла уведомлять об изменении scale. Кто-то изменил scale, тогда зачем этому кому-то об этом сообщать?
    emit(yScaleChanged(QString("y = ") + QString::number(scale_y * (height_pixels / grid_vertical_segments))));
}

void Screen::decreaseScaleY()
{
    scale_y *= scaling_factor_y;
    emit(yScaleChanged(QString("y = ") + QString::number(scale_y * (height_pixels / grid_vertical_segments))));
}

void Screen::increaseScaleX()
{
    //TODO: В моем представлении scale должен быть положительным целым числом. Развертка должна изменяться с шагом периода дискретизации и только с таким шагом

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
    //TODO: В моем представлении сдвиг должен быть положительным целым числом. Шаг сдвига = период дискретизации

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
    // TODO: эта строка оторвана от места где используется
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
    // TODO:Почему мы рассчитываем масштаб во время отрисовки. Почему мы не можем это делать при укладке данных в буфер экрана? при текущем подходе большая вычислительная нагрузка получается
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
    // TODO: зачем нам тут рассчитывать масштабы?

    QPainter painter(this);
    QPen gridPen(Qt::gray, 0, Qt::DotLine);
    painter.setPen(gridPen);

    int size = grid_horizontal_segments - 1;
    QLine vLines[size]; // TODO: массив в стеке в не хорошо. Функция критична к времени выполнения.
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
