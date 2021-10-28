#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
// TODO: этот комментарий для С++. В проекте желательно уменьшить зависимость файлов друг от друга. Не формальное правило в h файлов других инклюдов по возможности быть не должно.
// решается это неявным обьявлением классов или идеомой PIMPL
#include "udp.h"
#include "drawingprocessor.h"
#include "drawingsurface.h"
#include "Constants.h"

// TODO - это не нужно
QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }// TODO - это не нужно
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT

public:
    //TODO а что будет в софте еслия напишу
    // Controller a;
    // Controller b;
    // b = a;
    // или
    // Controller b(a);
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
    Ui::Controller *ui;// TODO - это не нужно
    Udp udp; // TODO - это не должно быть здесь
    DrawingProcessor drawingProcessor;
    DrawingSurface drawingSurface;
};
#endif // CONTROLLER_H
