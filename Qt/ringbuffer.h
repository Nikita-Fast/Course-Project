#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <array>
#include <qstring.h>
#include "QtDebug"

template<typename T, int capacity>
class RingBuffer
{
public:
    RingBuffer() {

    }

    T get(int index) {
        return buf[(head + offset + index) % capacity];
    }

    void put(T item) {
        buf[head] = item;
        head = (head + 1) % capacity;
    }

    void setOffset(int value) {
        offset = value;
    }

    void print() {
        QString str;
        for (int i = 0; i < capacity; i++) {
            str.append(QString::number(buf[i])).append(' ');
        }
        qDebug() << str;
    }

    void set(T value, int index) {
        buf[index] = value;
    }

    int getHead() {
        return head;
    }

    int getOldest() {
        return (head + 1) % capacity;
    }

private:
    std::array<T, capacity> buf;
    int head = 0;
    int offset = 0;

};

#endif // RINGBUFFER_H