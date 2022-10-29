#include <QtTest>
#include "StrictRingBuffer.h"

class TestRingBuffer : public QObject {
  Q_OBJECT

 public:
  TestRingBuffer();
  ~TestRingBuffer();

 private slots:
  void test_ring_buffer();
};

TestRingBuffer::TestRingBuffer() {}

TestRingBuffer::~TestRingBuffer() {}

void TestRingBuffer::test_ring_buffer() {
  int capacity = 3;
  StrictRingBuffer buf(3);
  QCOMPARE(buf.get_capacity(), capacity);

  buf.write_or_rewrite(13);

  QCOMPARE(buf.getElementsCount(), 1);
}

QTEST_APPLESS_MAIN(TestRingBuffer)

#include "main.moc"
