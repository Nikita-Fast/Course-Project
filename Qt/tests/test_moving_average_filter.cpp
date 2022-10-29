#include <QtTest>
#include "moving_average_filter.h"

// add necessary includes here

class TestMovingAverageFilter : public QObject {
  Q_OBJECT

 public:
  TestMovingAverageFilter();
  ~TestMovingAverageFilter();

 private slots:
  void test_moving_average_filter();
};

TestMovingAverageFilter::TestMovingAverageFilter() {}

TestMovingAverageFilter::~TestMovingAverageFilter() {}

void TestMovingAverageFilter::test_moving_average_filter() {
  int window_size = 16;
  MovingAverageFilter filter(window_size);

  QCOMPARE(9 - 1, 3 + 5);
}

// QTEST_APPLESS_MAIN(TestMovingAverageFilter)

#include "test_moving_average_filter.moc"
