#include "drawingprocessor.h"
#include "QtDebug"

DrawingProcessor::DrawingProcessor(QObject *parent) : QObject(parent)
{

}

void DrawingProcessor::enlargePixelSize()
{
    pixel_size_ms *= 1.5;
    emit(xScaleUpdated(pixel_size_ms));
    //processBuffer(curr_buffer);
    compressAxisX();
    emit(requestToDraw(points, pointsNumber));
}

void DrawingProcessor::compressAxisX() {
    time_offset_ms /= 1.5;
    for (int i = 0; i < pointsNumber; i++) {
        int x = points[i].x();
        points[i].setX(x / 1.5);
    }
    //нужно удалить точки у которых такие же х координаты как у предыдущих
}

void DrawingProcessor::stretchAxisX() {
    time_offset_ms *= 1.5;
    for (int i = 0; i < pointsNumber; i++) {
        int x = points[i].x();
        points[i].setX(x * 1.5);
    }
}

void DrawingProcessor::reducePixelSize()
{
    pixel_size_ms /= 1.5;
    emit(xScaleUpdated(pixel_size_ms));
    stretchAxisX();
    emit(requestToDraw(points, pointsNumber));
    //processBuffer(curr_buffer);
}

void DrawingProcessor::enlargeScaleY()
{
    y_scale /= 1.25;
    emit(yScaleUpdated(y_scale));
    //processBuffer(curr_buffer);
}

void DrawingProcessor::reduceScaleY()
{
    y_scale *= 1.25;
    emit(yScaleUpdated(y_scale));
    //processBuffer(curr_buffer);
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
    double window_size_ms = WINDOW_WIDTH * pixel_size_ms;
    if (time_offset_ms >= window_size_ms) {
        time_offset_ms -= (window_size_ms / MAGIC_CONST); //из-за сдвига, освободится место, значит offset уменьшится
        shiftDrawingsToLeft();
    }
    double free_space_ms = window_size_ms - time_offset_ms;

    double buffer_size_ms = (BUFFER_SIZE - samples_processed) * SAMPLING_INTERVAL_MS;
    double interval_to_process_ms = std::min(free_space_ms, buffer_size_ms);

    double step_ms = std::max(pixel_size_ms, SAMPLING_INTERVAL_MS);

    double max_t = 0.0;
    for (double t = 0.0; t < interval_to_process_ms; t += step_ms) {
        int y = buffer[samples_processed + (int)(t / SAMPLING_INTERVAL_MS)];
        int y_in_pixels = WINDOW_HEIGHT / 2 - y / y_scale;

        double t_ms = time_offset_ms + t;
        int pix_num = t_ms / pixel_size_ms;

        points[pointsNumber++] = QPoint(pix_num, y_in_pixels);
        max_t = t;
    }
    //правильно обновить time_offset_ms
    double size = max_t + step_ms;

    samples_processed += size / SAMPLING_INTERVAL_MS;
    continue_drawing_buffer = samples_processed < BUFFER_SIZE;
    samples_processed %= BUFFER_SIZE;

    time_offset_ms += size;

    emit(requestToDraw(points, pointsNumber));
}


void DrawingProcessor::requestNewDrawing()
{
    //prepareData();
    if (continue_drawing_buffer) {
        processBuffer(curr_buffer);
    }
    else {
        emit(bufferProcessed(curr_buffer));
    }
}



//void DrawingProcessor::prepareData()
//{
//    if (iteration < helper_length) {
//        int index = iteration == 0 ? 0 : helper_array[iteration - 1] + 1;
//        int length = helper_array[iteration] - index + 1;
//        emit(requestToDraw(points + index, length));
//        iteration++;
//    }
//    else {
//        emit(bufferProcessed(curr_buffer));
//    }
//}

//void DrawingProcessor::helper()
//{
//    iteration = 0;
//    int max = points[0].x();
//    helper_length = 0;

//    int k = 0;
//    for (int i = 1; i < pointsNumber; i++) {
//        if (points[i].x() >= max) {
//            max = points[i].x();

//        }
//        else {
//            helper_array[k++] = i - 1;
//            helper_length++;
//            max = points[i].x();
//        }
//    }
//    helper_array[k] = pointsNumber - 1;
//    helper_length++;
//}

//void DrawingProcessor::shiftToLeft(int pixels) {
//    for (int i = 0; i < pointsNumber; i++) {
//        int curr_x = points[i].x();
//        points[i].setX(curr_x - pixels);
//    }
//    helper();
//    prepareData();
//}

//void DrawingProcessor::shiftToRight(int pixels) {
//    for (int i = 0; i < pointsNumber; i++) {
//        int curr_x = points[i].x();
//        points[i].setX(curr_x + pixels);
//    }
//    helper();
//    prepareData();
//}

void DrawingProcessor::shiftDrawingsToLeft() {
    for (int i = 0; i < pointsNumber; i++) {
        int old_x = points[i].x();
        points[i].setX(old_x - WINDOW_WIDTH / MAGIC_CONST);
    }
    int num = 0;
    for (int i = 0; i < pointsNumber; i++) {
        if (points[i].x() < 0) {
            num++;
        }
        else {
            break;
        }
    }

    memmove(points, points + num, sizeof(QPoint) * (pointsNumber - num)); // TODO - сдвиг памяти может быть не удачным решением. Это открытый вопрос. Может быть надо использовать циклическую индексацию
    pointsNumber -= num;
}

