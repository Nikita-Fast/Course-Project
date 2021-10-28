#ifndef CONSTANTS_H
#define CONSTANTS_H

// TODO: Глобальные константы - это не есть хорошо. По возможности надо исключать зависимости между сущностями в программе.
// TODO: Часть параметров - это экран (у нас может быть два экрана с разными значениями этих параметров), другая часть - это параметры сущности обработки данных
const int BUFFER_SIZE = 512 * 4;
const int PORT = 50001;
const int SAMPLE_RATE = 2048 * 4;
const double SAMPLING_INTERVAL_MS = 1000.0 / SAMPLE_RATE;
const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 750;
const int HORIZ_SEGMENTS = 10;
const int VERT_SEGMENTS = 4;
const int MAGIC_CONST = 32;


#endif // CONSTANTS_H
