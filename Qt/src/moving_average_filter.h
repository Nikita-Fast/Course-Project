#ifndef MOVINGAVERAGEFILTER_H
#define MOVINGAVERAGEFILTER_H
#include <vector>

class StrictRingBuffer;

class MovingAverageFilter {
 public:
  MovingAverageFilter(int window_size);
  void filter(std::vector<short>& data);

 private:
  StrictRingBuffer* window;
};

#endif  // MOVINGAVERAGEFILTER_H
