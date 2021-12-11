#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>
#include "nonmoveable.h"

class DataProcessor;
class Screen;
class DataInterface;

class Oscilloscope final : public QWidget, NonMoveable<Oscilloscope>
{
    Q_OBJECT

public:
    explicit Oscilloscope(QWidget *parent = nullptr);
    ~Oscilloscope();

private:
    Screen* screen;
    DataProcessor* processor;
    DataInterface* dataInterface;
};
#endif // OSCILLOSCOPE_H
