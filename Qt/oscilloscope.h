#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>
#include "dataprocessor.h"
#include "screen.h"
#include "datainterface.h"

class Oscilloscope : public QWidget
{
    Q_OBJECT

public:
    Oscilloscope(QWidget *parent = nullptr);
    ~Oscilloscope();

private:
    DataProcessor processor;
    Screen screen;
    DataInterface *dataInterface;
};
#endif // OSCILLOSCOPE_H
