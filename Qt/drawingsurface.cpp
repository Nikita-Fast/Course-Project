#include "drawingsurface.h"
#include "qpainter.h"
#include "QtDebug"
#include <QThread>

DrawingSurface::DrawingSurface(QWidget *parent) : QWidget(parent)
{
    setGeometry(30, 30, WINDOW_WIDTH, WINDOW_HEIGHT);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
}

void DrawingSurface::receiveDrawingRequest(QPoint *points, int length)
{
    this->points = points;
    this->length = length;
    update();
}

void DrawingSurface::paintEvent(QPaintEvent *)
{
    drawGrid();
    QPainter painter(this);
    QPen pointPen(Qt::cyan);
    pointPen.setWidth(3);
    painter.setPen(pointPen);

    if (length > 0 && points != nullptr) {
        //QThread::currentThread()->msleep(33);

        painter.drawPolyline(points, length);

        emit(pointsDrawn());
    }
}

void DrawingSurface::drawGrid() {
    QPainter painter(this);
    QPen gridPen(Qt::white);
    gridPen.setWidth(1);
    painter.setPen(gridPen);

    int hSegments = HORIZ_SEGMENTS;
    int size = hSegments - 1;
    QLine vLines[size];
    int hStep = width() / hSegments;

    for (int i = 0; i < size; i++) {
        vLines[i] = QLine(hStep * (i + 1), 0, hStep * (i + 1), height() - 1);
    }
    painter.drawLines(vLines, size);

    int vSegments = VERT_SEGMENTS;
    int size2 = vSegments - 1;
    QLine hLines[size2];
    int vStep = height() / vSegments;

    for (int i = 0; i < size2; i++) {
        hLines[i] = QLine(0, vStep * (i + 1), width() - 1,  vStep * (i + 1));
    }
    painter.drawLines(hLines, size2);
}
