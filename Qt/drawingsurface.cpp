#include "drawingsurface.h"
#include "qpainter.h"

DrawingSurface::DrawingSurface(QWidget *parent) : QWidget(parent)
{

    setFixedSize(750, 500);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
}

void DrawingSurface::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pointPen(Qt::cyan);
    pointPen.setWidth(3);
    painter.setPen(pointPen);
    painter.drawPolyline(myPoints.data(), static_cast<int>(myPoints.size()));
}

std::vector<QPoint> &DrawingSurface::getMyPoints()
{
    return myPoints;
}

void DrawingSurface::clearButtonClicked()
{
    myPoints.clear();
    update();
}
