#ifndef STRICTRINGBUFFER_H
#define STRICTRINGBUFFER_H

#include "QtDebug"

class StrictRingBuffer {
 public:
  StrictRingBuffer(int capacity) {
    this->capacity = capacity;
    buf = new short[capacity];
    memset(buf, 0, capacity * sizeof(short));
  }

  ~StrictRingBuffer() { delete buf; }

  int sum() {
    int sum = 0;
    for (int i = 0; i < capacity; i++) {
      sum += buf[i];
    }
    return sum;
  }

  void write(short value) {
    if (isFull()) {
      qDebug() << "can't write values to a full buffer";
    } else {
      buf[w_index] = value;
      w_index = (w_index + 1) % capacity;
      el_cnt++;
    }
  }

  void write_or_rewrite(short value) {
    buf[w_index] = value;
    w_index = (w_index + 1) % capacity;
    el_cnt++;
  }

  void write(short* samples, int cnt) {
    if (capacity - getElementsCount() >= cnt) {
      int toEnd =
          capacity - w_index;  //число эл-ов до перехода по кольцу в начало
      int n = std::min(toEnd, cnt);
      std::copy(samples, samples + n, buf + w_index);
      w_index = (w_index + n) % capacity;

      if (n < cnt) {
        n = cnt - n;
        std::copy(samples + n + 1, samples + cnt, buf + w_index);
        w_index = (w_index + n) % capacity;
      }
      el_cnt += cnt;
    } else {
      qDebug() << "not enough place in buffer!";
    }
  }

  //    short read() {
  //        if (isEmpty()) {
  //            qDebug() << "can't read from empty buffer";
  //        }
  //        else {
  //            short value = buf[r_index];
  //            r_index = (r_index + 1) % capacity;
  //            el_cnt--;
  //            return value;
  //        }
  //    }

  void read(short* dst, int cnt) {
    if (getElementsCount() < cnt) {
      qDebug() << "not enough values in buffer";
    } else {
      int toEnd = capacity - r_index;
      int n = std::min(toEnd, cnt);

      std::copy(buf + r_index, buf + r_index + n, dst);

      r_index = (r_index + n) % capacity;

      if (n < cnt) {
        int k = cnt - n;
        std::copy(buf + r_index, buf + r_index + k, dst + n);
        r_index = (r_index + k) % capacity;
      }
      el_cnt -= cnt;
    }
  }

  short peekAt(int n) { return buf[n % capacity]; }

  int getElementsCount() { return el_cnt; }

  bool isFull() { return el_cnt == capacity; }

  bool isEmpty() { return el_cnt == 0; }

  int get_capacity() { return capacity; }
  int get_w_index() { return w_index; }

 private:
  int capacity;
  short* buf;
  int w_index = 0;
  int r_index = 0;
  int el_cnt = 0;
};

#endif  // STRICTRINGBUFFER_H
