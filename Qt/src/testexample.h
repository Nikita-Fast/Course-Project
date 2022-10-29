#include <QObject>
#include <QtTest/QTest>

class TestExample : public QObject {
  Q_OBJECT

 private slots:
  void add();
};

void TestExample::add() {
  int actual = 2 + 3;
  QCOMPARE(5, actual);
}
