#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), blackboard(this)
{
    ui->setupUi(this);
    blackboard.setGeometry(30, 40, width(), height());
    connect(ui->clearBtn, &QPushButton::clicked, &blackboard, &DrawingSurface::clearButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    blackboard.update();
}

void MainWindow::addPoint(QPoint point) {
    blackboard.getMyPoints().push_back(point);
}

int MainWindow::getBlackboardHeight() {
    return blackboard.height();
}

