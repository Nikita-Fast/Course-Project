#include "oscilloscope.h"
#include "StrictRingBuffer.h"
#include "datainterface.h"
#include "dataprocessor.h"
#include "screen.h"

#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "udpinterface.h"
#include "uartinterface.h"

#include <QResizeEvent>

Oscilloscope::Oscilloscope(QWidget* parent)
    : QMainWindow(parent),
      screen(new Screen(OSCILL_FREQ_HZ, this)),
      processor(new DataProcessor(OSCILL_FREQ_HZ)),
      dataInterface(new UdpInterface)  // TODO: это внешний интерфейс
{
  buffer = new StrictRingBuffer(BUFFER_SIZE);
  screen->set_buffer(buffer);
  processor->setBuffer(buffer);

  QGroupBox* control_group = new QGroupBox("Панель управления", this);
  QVBoxLayout* btns_lbls_layout = new QVBoxLayout(control_group);
  QPushButton* inc_scale_y_btn = new QPushButton("scale_y++");
  QPushButton* dec_scale_y_btn = new QPushButton("scale_y--");
  QPushButton* left_shift_btn = new QPushButton("left_shift");
  QPushButton* right_shift_btn = new QPushButton("right_shift");
  QPushButton* up_shift_btn = new QPushButton("up_shift");
  QPushButton* down_shift_btn = new QPushButton("down_shift");
  QPushButton* inc_scale_x_btn = new QPushButton("scale_x++");
  QPushButton* dec_scale_x_btn = new QPushButton("scale_x--");
  QPushButton* pause_btn = new QPushButton("pause");
  QPushButton* start_btn = new QPushButton("start");
  QLabel* scale_y_label = new QLabel("y: 15000");
  QLabel* scale_x_label = new QLabel("x: ");

  // inc_scale_y_btn->setFixedWidth(150);
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

  setCentralWidget(screen);

  QDockWidget* panel = new QDockWidget(this);
  panel->setWidget(control_group);
  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, panel);

  connect(dataInterface, SIGNAL(packetSent(short*, int)), processor,
          SLOT(writePacketToBuf(short*, int)));
  //  connect(processor, SIGNAL(sendFrameToScreen(short*, int)), screen,
  //          SLOT(receiveFrame(short*, int)));

  connect(inc_scale_y_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleY()));
  connect(dec_scale_y_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleY()));
  connect(inc_scale_x_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleX()));
  connect(dec_scale_x_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleX()));
  connect(left_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToLeft()));
  connect(right_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToRight()));
  connect(up_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftUp()));
  connect(down_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftDown()));
  connect(screen, SIGNAL(send_v_grid_step(int)), scale_y_label,
          SLOT(setNum(int)));
  connect(screen, SIGNAL(send_h_grid_step(int)), scale_x_label,
          SLOT(setNum(int)));
  connect(pause_btn, SIGNAL(clicked()), processor, SLOT(set_is_paused_true()));
  connect(start_btn, SIGNAL(clicked()), processor, SLOT(set_is_paused_false()));

  //screen->installEventFilter(this);
  installEventFilter(this);
}

Oscilloscope::~Oscilloscope() {
    delete buffer;
}

void Oscilloscope::resizeEvent(QResizeEvent *event)
{
//      const QSize sizeEventOld = event->oldSize();
//      const QSize sizeEvent = event->size();
//    //  qDebug() << sizeEventOld << ", " << sizeEvent;

//      if (screen-> get_rendered_part_start() + sizeEvent.width() * screen->get_x_scale() > buffer->get_capacity()) {
//          qDebug() << "went out of buffer";
//            resize(sizeEventOld);
//      }
//      else {
//          QWidget::resizeEvent(event);
//      }
    QWidget::resizeEvent(event);
}

bool Oscilloscope::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent *resize_event = static_cast<QResizeEvent *>(event);
        const QSize sizeEventOld = resize_event->oldSize();
        const QSize sizeEvent = resize_event->size();
         qDebug() << sizeEventOld << ", " << sizeEvent;

        if (screen->get_rendered_part_start() + sizeEvent.width() * screen->get_x_scale() > buffer->get_capacity()) {
//            int max_w = (buffer->get_capacity() - screen->get_rendered_part_start()) / screen->get_x_scale();
//            screen->resize(max_w, 500);
            resize(sizeEventOld);
            return true;
        }
        else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    }
}
