#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QtCore>
#include <QtGui>
#include "drawingsurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addPoint(QPoint point);
    int getBlackboardHeight();

private:
    Ui::MainWindow *ui;
    DrawingSurface blackboard;

protected:
    void paintEvent(QPaintEvent *e);
};
#endif // MAINWINDOW_H
