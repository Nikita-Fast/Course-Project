#include "StrictRingBuffer.h"
#include "moving_average_filter.h"

MovingAverageFilter::MovingAverageFilter(int window_size) {
  window = new StrictRingBuffer(window_size);
}

void MovingAverageFilter::filter(std::vector<short>& data) {
  for (size_t i = 0; i < data.size(); i++) {
    window->write_or_rewrite(data[i]);
    short averaged = window->sum() / window->get_capacity();
    data[i] = averaged;
  }
}
