#include "oscilloscope.h"
#include "dataprocessor.h"
#include "screen.h"
#include "datainterface.h"

#include "udpinterface.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

Oscilloscope::Oscilloscope(QWidget *parent)
    : QWidget(parent),
      screen(new Screen(this)),
      processor(new DataProcessor()),
      dataInterface(new UdpInterface)
{
    QVBoxLayout *btns_lbls_layout = new QVBoxLayout;

    QPushButton *inc_scale_y_btn = new QPushButton("scale_y++");
    QPushButton *dec_scale_y_btn = new QPushButton("scale_y--");
    QPushButton *left_shift_btn = new QPushButton("left_shift");
    QPushButton *right_shift_btn = new QPushButton("right_shift");
    QPushButton *up_shift_btn = new QPushButton("up_shift");
    QPushButton *down_shift_btn = new QPushButton("down_shift");
    QPushButton *inc_scale_x_btn = new QPushButton("scale_x++");
    QPushButton *dec_scale_x_btn = new QPushButton("scale_x--");
    QPushButton *pause_btn = new QPushButton("pause");
    QPushButton *start_btn = new QPushButton("start");
    QLabel *scale_y_label = new QLabel("y: 15000");
    QLabel *scale_x_label = new QLabel("x: ");

    inc_scale_y_btn->setFixedWidth(150);
    btns_lbls_layout->addWidget(inc_scale_x_btn);
    btns_lbls_layout->addWidget(dec_scale_x_btn);
    btns_lbls_layout->addWidget(inc_scale_y_btn);
    btns_lbls_layout->addWidget(dec_scale_y_btn);
    btns_lbls_layout->addWidget(left_shift_btn);
    btns_lbls_layout->addWidget(right_shift_btn);
    btns_lbls_layout->addWidget(up_shift_btn);
    btns_lbls_layout->addWidget(down_shift_btn);
    btns_lbls_layout->addWidget(pause_btn);
    btns_lbls_layout->addWidget(start_btn);
    btns_lbls_layout->addWidget(scale_y_label);
    btns_lbls_layout->addWidget(scale_x_label);

    QHBoxLayout *main_layout = new QHBoxLayout;
    main_layout->addWidget(screen);
    main_layout->addLayout(btns_lbls_layout);
    setLayout(main_layout);

    //connect(dataInterface, SIGNAL(packetSent(short *, int)), processor, SLOT(storePacket(short *, int)));
    //connect(screen, SIGNAL(getNewValues(int)), processor, SLOT(sendDataToScreen(int)));
    //connect(processor, SIGNAL(sendSamples(short*, int)), screen, SLOT(updateScreenBuffer(short*, int)));
    //connect(dataInterface, SIGNAL(packetSent(short *, int)), screen, SLOT(storePacket(short *, int)));

    connect(dataInterface, SIGNAL(packetSent(short *, int)), processor, SLOT(writePacketToBuf(short *, int)));
    //sendFrameToScreen(short *, int)
    connect(processor, SIGNAL(sendFrameToScreen(short *, int)), screen, SLOT(receiveFrame(short *, int)));

    connect(inc_scale_y_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleY()));
    connect(dec_scale_y_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleY()));
    connect(inc_scale_x_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleX()));
    connect(dec_scale_x_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleX()));
    connect(left_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToLeft()));
    connect(right_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToRight()));
    connect(up_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftUp()));
    connect(down_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftDown()));
    connect(screen, SIGNAL(yScaleChanged(QString)), scale_y_label, SLOT(setText(QString)));
    connect(screen, SIGNAL(xScaleChanged(QString)), scale_x_label, SLOT(setText(QString)));
    connect(pause_btn, SIGNAL(clicked()), processor, SLOT(stopBuffTimer()));
    connect(start_btn, SIGNAL(clicked()), processor, SLOT(startBuffTimer()));
}

Oscilloscope::~Oscilloscope()
{
    delete dataInterface;
    delete processor;
    delete screen;
}