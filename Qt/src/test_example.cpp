#include <QObject>
#include <QtTest/QtTest>

class TestExample : public QObject {
  Q_OBJECT

 private slots:
  void test_add();
};

void TestExample::test_add() {
  int actual = 2 + 3;
  QCOMPARE(5, actual);
}

// QTEST_MAIN(TestExample)
#include "test_example.moc"
