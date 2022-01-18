#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <array>
#include <qstring.h>
#include "QtDebug"

template<typename T, int capacity>
class RingBuffer
{
public:
    RingBuffer() = default;

    T get(int index) {
        return buf[(head + offset + index) % capacity];
    }

    void put(T item) {
        buf[head] = item;
        head = (head + 1) % capacity;
    }

    // TODO: плохой метод
    void setOffset(int value) {
        offset = value;
    }
    // TODO: плохой метод
    void set(T value, int index) {
        buf[index] = value;
    }
    // TODO: плохой метод
    int getHead() {
        return head;
    }
    int getOldest() {
        return (head + 1) % capacity;
    }

    void print() {
        QString str;
        for (int i = 0; i < capacity; i++) {
            str.append(QString::number(buf[i])).append(' ');
        }
        qDebug() << str;
    }

private:
    std::array<T, capacity> buf;
    int head = 0;
    int offset = 0;

};

#endif // RINGBUFFER_H
