#ifndef DRAWINGSURFACE_H
#define DRAWINGSURFACE_H

#include <QWidget>
#include "Constants.h"

class DrawingSurface : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingSurface(QWidget *parent = nullptr);

signals:
    void pointsDrawn();

public slots:
    void receiveDrawingRequest(QPoint *points, int length);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPoint *points = nullptr;
    int length = 0;
    void drawGrid();

};

#endif // DRAWINGSURFACE_H
