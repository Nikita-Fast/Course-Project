#include "controller.h"
#include "ui_controller.h"

Controller::Controller(QWidget *parent)
    : QWidget(parent), ui(new Ui::Controller), drawingSurface(this)
{
    ui->setupUi(this);

    connect(&udp, SIGNAL(bufferIsFull(short *)), &drawingProcessor, SLOT(processBuffer(short *)));

    connect(&drawingProcessor, SIGNAL(bufferProcessed(short *)), &udp, SLOT(unlockBuffer(short *)));

    connect(&drawingProcessor, SIGNAL(requestToDraw(QPoint *, int)),
            &drawingSurface, SLOT(receiveDrawingRequest(QPoint *, int)));
    connect(&drawingSurface, SIGNAL(pointsDrawn()), &drawingProcessor, SLOT(requestNewDrawing()));

    connect(&drawingProcessor, SIGNAL(xScaleUpdated(double)), this, SLOT(xScaleUpdated(double)));
    connect(&drawingProcessor, SIGNAL(yScaleUpdated(double)), this, SLOT(yScaleUpdated(double)));
}

Controller::~Controller()
{
    delete ui;
}

void Controller::xScaleUpdated(double x_scale) {
    std::string txt = "x: " + std::to_string(x_scale * WINDOW_WIDTH / HORIZ_SEGMENTS) + "ms";

    ui->label->setText(QString::fromStdString(txt));
}

void Controller::yScaleUpdated(double y_scale) {
    std::string txt = "y: " + std::to_string(y_scale * WINDOW_HEIGHT / VERT_SEGMENTS);

    ui->label_2->setText(QString::fromStdString(txt));
}


void Controller::on_pushButton_clicked()
{
    drawingProcessor.reducePixelSize();
}


void Controller::on_pushButton_2_clicked()
{
    drawingProcessor.enlargePixelSize();
}


void Controller::on_pushButton_3_clicked()
{
    drawingProcessor.enlargeScaleY();
}


void Controller::on_pushButton_4_clicked()
{
    drawingProcessor.reduceScaleY();
}

