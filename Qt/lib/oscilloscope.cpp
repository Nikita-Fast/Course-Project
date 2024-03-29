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
#include <QHBoxLayout>
#include "udpinterface.h"
#include "uartinterface.h"

#include <QResizeEvent>
#include <QSizePolicy>
#include <QLineEdit>
#include <QIntValidator>
#include <QComboBox>

Oscilloscope::Oscilloscope(QWidget* parent) :
      screen(new Screen(OSCILL_FREQ_HZ, this)),
      processor(new DataProcessor(OSCILL_FREQ_HZ)),
      dataInterface(new UdpInterface)  // TODO: это внешний интерфейс
{
  buffer = new StrictRingBuffer(BUFFER_SIZE);
  screen->set_buffer(buffer);
  processor->setBuffer(buffer);

  QGroupBox* control_group = new QGroupBox("Control panel", this);
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
  QPushButton* trigger_btn = new QPushButton("trigger");
  QLabel* trigger_mode_lbl = new QLabel("trigger mode:");

  QValidator *validator = new QIntValidator(-32768, 32767, this);
  trigger_level = new QLineEdit;
  trigger_level->setValidator(validator);
  trigger_level->setPlaceholderText("trigger lvl");

  QValidator *trig_offset_validator = new QIntValidator(0, BUFFER_SIZE - 1, this);
  trigger_offset = new QLineEdit;
  trigger_offset->setValidator(trig_offset_validator);
  trigger_offset->setPlaceholderText("trigger offset");

  trigger_mode = new QComboBox(this);
  trigger_mode->addItem("continious");
  trigger_mode->addItem("one-time");

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
  btns_lbls_layout->addWidget(trigger_btn);
  btns_lbls_layout->addWidget(trigger_mode_lbl);
  btns_lbls_layout->addWidget(trigger_mode);
  btns_lbls_layout->addWidget(trigger_level);
  btns_lbls_layout->addWidget(trigger_offset);
  btns_lbls_layout->addStretch();

  setCentralWidget(screen);

  QDockWidget* panel = new QDockWidget(this);
  panel->setWidget(control_group);
  panel->setMinimumWidth(141);
  panel->setMaximumWidth(141);
  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, panel);

  connect(dataInterface, SIGNAL(packetSent(short*, int)), processor,
          SLOT(writePacketToBuf(short*, int)));

  connect(inc_scale_y_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleY()));
  connect(dec_scale_y_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleY()));
  connect(inc_scale_x_btn, SIGNAL(clicked()), screen, SLOT(increaseScaleX()));
  connect(dec_scale_x_btn, SIGNAL(clicked()), screen, SLOT(decreaseScaleX()));
  connect(left_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToLeft()));
  connect(right_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftToRight()));
  connect(up_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftUp()));
  connect(down_shift_btn, SIGNAL(clicked()), screen, SLOT(shiftDown()));
  connect(pause_btn, SIGNAL(clicked()), processor, SLOT(set_is_paused_true()));
  connect(start_btn, SIGNAL(clicked()), processor, SLOT(set_is_paused_false()));

  connect(trigger_btn, SIGNAL(clicked()), processor, SLOT(toggle_trigger()));
  connect(trigger_btn, SIGNAL(clicked()), screen, SLOT(toggle_trigger()));
  connect(trigger_level, SIGNAL(returnPressed()), this, SLOT(read_trigger_level()));
  connect(this, SIGNAL(trigger_lvl_updated(int)), processor, SLOT(set_trigger_level(int)));
  connect(this, SIGNAL(trigger_lvl_updated(int)), screen, SLOT(set_trigger_level(int)));
  connect(trigger_mode, SIGNAL(currentIndexChanged(int)), processor, SLOT(change_trigger_mode(int)));

  connect(trigger_offset, SIGNAL(returnPressed()), this, SLOT(read_trigger_offset()));
  connect(this, SIGNAL(trigger_offset_updated(int)), processor, SLOT(set_trigger_offset(int)));
  connect(this, SIGNAL(trigger_offset_updated(int)), screen, SLOT(set_trigger_offset(int)));



  connect(screen, SIGNAL(update_max_width(int)), this, SLOT(update_max_width(int)));
}

Oscilloscope::~Oscilloscope() {
    delete buffer;
}

void Oscilloscope::update_max_width(int curr_max_screen_width)
{
    int pixels_for_buttons = 141;
    setMaximumWidth(curr_max_screen_width + pixels_for_buttons);
}

void Oscilloscope::read_trigger_level()
{
    auto s = trigger_level->text();
    emit(trigger_lvl_updated(s.toInt()));
}

void Oscilloscope::read_trigger_offset()
{
    auto offset = trigger_offset->text();
    emit(trigger_offset_updated(offset.toInt()));
}

void Oscilloscope::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent* ev = static_cast<QWindowStateChangeEvent*>(e);
        if (ev->oldState() == Qt::WindowNoState && width() <= 1840) {
            qDebug() << "broken full screen" << screen->width() << 1841 - screen->width();
            int shift_px = 1841 - screen->width();
            screen->shift_rendered_part_start(shift_px * screen->get_x_scale());
            qDebug() << "max_w after shift: " << maximumWidth();
            resize(maximumWidth(), height());
        }
    }
}
