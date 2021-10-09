#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pointPen(Qt::red);
    pointPen.setWidth(3);
    painter.setPen(pointPen);

    painter.drawPolyline(myPoints.data(), static_cast<int>(myPoints.size()));
}


void MainWindow::on_pushButton_clicked()
{
    myPoints.clear();
    update();
}

std::vector<QPoint> &MainWindow::getMyPoints()
{
    return myPoints;
}

