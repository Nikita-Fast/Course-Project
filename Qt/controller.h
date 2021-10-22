#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include "udp.h"
#include "drawingprocessor.h"
#include "drawingsurface.h"
#include "Constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT

public:
    Controller(QWidget *parent = nullptr);
    ~Controller();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void xScaleUpdated(double x_scale);
    void yScaleUpdated(double y_scale);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Controller *ui;
    Udp udp;
    DrawingProcessor drawingProcessor;
    DrawingSurface drawingSurface;
};
#endif // CONTROLLER_H
