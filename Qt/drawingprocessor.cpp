#include "drawingprocessor.h"
#include "QtDebug"

DrawingProcessor::DrawingProcessor(QObject *parent) : QObject(parent)
{

}

void DrawingProcessor::enlargePixelSize()
{
    pixel_size_ms *= 1.5;
    emit(xScaleUpdated(pixel_size_ms));
}

void DrawingProcessor::reducePixelSize()
{
    pixel_size_ms /= 1.5;
    emit(xScaleUpdated(pixel_size_ms));
}

void DrawingProcessor::enlargeScaleY()
{
    y_scale /= 1.25;
    emit(yScaleUpdated(y_scale));
}

void DrawingProcessor::reduceScaleY()
{
    y_scale *= 1.25;
    emit(yScaleUpdated(y_scale));
}

int normalize(double t, int base) {
    while (t >= base) {
        t -= base;
    }
    return t;
}

double normalize(double t, double base) {
    while (t >= base) {
        t -= base;
    }
    return t;
}

void DrawingProcessor::processBuffer(short *buffer)
{
    curr_buffer = buffer;
    pointsNumber = 0;

    double density = pixel_size_ms / SAMPLING_INTERVAL_MS; //число сэмплов на один пиксель в текущем масштабе

    double step = std::max(1.0, density);
    double future_buffer_offset_ms = 0.0;
    int remainder = 0;

    for (double i = 0; i < BUFFER_SIZE; i += step) {
        int y;
        if (density >= 1.0) {
            y = buffer[sample_offset + (int)i];
        }
        else {
            y = buffer[(int)i];
        }

        int y_in_pixels = WINDOW_HEIGHT / 2 - y / y_scale;

        double t_ms = normalize(prev_buffer_offset_ms + i * SAMPLING_INTERVAL_MS, WINDOW_WIDTH * pixel_size_ms);
        future_buffer_offset_ms = normalize(t_ms + step * SAMPLING_INTERVAL_MS, WINDOW_WIDTH * pixel_size_ms);

        int pix_num = normalize(t_ms / pixel_size_ms, WINDOW_WIDTH);

        points[pointsNumber++] = QPoint(pix_num, y_in_pixels);

        remainder = BUFFER_SIZE - (int)i;
    }
    prev_buffer_offset_ms = future_buffer_offset_ms;
    sample_offset = density - remainder;

    helper();
    prepareData();
}

void DrawingProcessor::requestNewDrawing()
{
    prepareData();
}



void DrawingProcessor::prepareData()
{
    if (iteration < helper_length) {
        int index = iteration == 0 ? 0 : helper_array[iteration - 1] + 1;
        int length = helper_array[iteration] - index + 1;
        emit(requestToDraw(points + index, length));
        iteration++;
    }
    else {
        emit(bufferProcessed(curr_buffer));
    }
}

void DrawingProcessor::helper()
{
    iteration = 0;
    int max = points[0].x();
    helper_length = 0;

    int k = 0;
    for (int i = 1; i < pointsNumber; i++) {
        if (points[i].x() >= max) {
            max = points[i].x();

        }
        else {
            helper_array[k++] = i - 1;
            helper_length++;
            max = points[i].x();
        }
    }
    helper_array[k] = pointsNumber - 1;
    helper_length++;
}

