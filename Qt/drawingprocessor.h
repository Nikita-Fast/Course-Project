#ifndef DRAWINGPROCESSOR_H
#define DRAWINGPROCESSOR_H

#include <QObject>
#include "Constants.h"
#include <QPoint>

class DrawingProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DrawingProcessor(QObject *parent = nullptr);

    void enlargePixelSize();
    void reducePixelSize();
    void enlargeScaleY();
    void reduceScaleY();

signals:
    void bufferProcessed(short *buffer);
    void requestToDraw(QPoint *points, int length);

    void xScaleUpdated(double x_scale);
    void yScaleUpdated(double y_scale);

public slots:
    void processBuffer(short *buffer);
    void requestNewDrawing();

private:
    short *curr_buffer;

    double pixel_size_ms = 1;
    double y_scale = 200;
    QPoint points[BUFFER_SIZE];
    int pointsNumber = 0;
    int sample_offset = 0;
    double prev_buffer_offset_ms = 0.0;

    void prepareData();

    void helper();
    int max_x = 0;
    int helper_array[BUFFER_SIZE];
    int helper_length = 0;
    int iteration = 0;

};

#endif // DRAWINGPROCESSOR_H
