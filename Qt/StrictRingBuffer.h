#ifndef STRICTRINGBUFFER_H
#define STRICTRINGBUFFER_H

#include "QtDebug"

class StrictRingBuffer {

public:

    StrictRingBuffer(int capacity) {
        this->capacity = capacity;
        buf = new short[capacity];
    }

    ~StrictRingBuffer() {
        delete buf;
    }

    void write(short value) {
        if (isFull()) {
            qDebug() << "can't write values to a full buffer";
        }
        else {
            buf[w_index] = value;
            w_index = (w_index + 1) % capacity;
            el_cnt++; //mutex?
        }
    }

    void write(short *samples, int cnt) {
        if (capacity - getElementsCount() >= cnt) {
            int toEnd = capacity - w_index; //число эл-ов до перехода по кольцу в начало
            int n = std::min(toEnd, cnt);
            std::copy(samples, samples + n, buf + w_index);
            w_index = (w_index + n) % capacity;

            if (n < cnt) {
                n = cnt - n;
                std::copy(samples + n + 1, samples + cnt, buf + w_index);
                w_index = (w_index + n) % capacity;
            }
            el_cnt += cnt;
        }
        else {
            qDebug() << "not enough place in buffer!";
        }
    }

    short read() {
        if (isEmpty()) {
            qDebug() << "can't read from empty buffer";
        }
        else {
            short value = buf[r_index];
            r_index = (r_index + 1) % capacity;
            el_cnt--;
            return value;
        }
    }

    int getElementsCount() {
        return el_cnt;
    }

    bool isFull() {
        return el_cnt == capacity;
    }

    bool isEmpty() {
        return el_cnt == 0;
    }


private:
    int capacity = 0;
    short *buf;
    int w_index = 0;
    int r_index = 0;
    int el_cnt = 0;
};

#endif // STRICTRINGBUFFER_H
