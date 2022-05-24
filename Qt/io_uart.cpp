#include "io_uart.h"
//#include "TR.h"
#include "CRC16.h"
//#include "gui_uart_log.h"

//#include "global_repeater.h"
#include "qdebug.h"

using namespace io;
Uart::Uart(QString Uart_Name, QObject* parent)
    : QObject(parent), rx_buffer(MAX_RX_BUFFER_SIZE, 0), uart_name(Uart_Name) {
  num_rx_bytes = 0;
  index = 0;
  corrector = 0;
  port.setBaudRate(460800);
  port.setParity(QSerialPort::Parity::NoParity);
  port.setFlowControl(QSerialPort::FlowControl::NoFlowControl);
  port.setDataBits(QSerialPort::DataBits::Data8);
  port.setStopBits(QSerialPort::OneStop);
  port.setReadBufferSize(2e6);

  connect(&port, SIGNAL(readyRead()), this, SLOT(read_from_uart()));
}

Uart::~Uart() {
  close();
}

bool Uart::isOpen() const {
  return port.isOpen();
}

bool Uart::setBaudRate(qint32 rate) {
  return port.setBaudRate(rate);
}

void Uart::setPortName(const QString& name) {
  port.setPortName(name);
}

QString Uart::getPortName() const {
  return port.portName();
}

bool Uart::open() {
  if (port.isOpen())
    return false;

  if (port.open(QIODevice::ReadWrite)) {
    port.setRequestToSend(false);
    port.setDataTerminalReady(false);
  } else {
    QString str = port.errorString();
    emit error(str);
  }
  QString str = port.errorString();
  index = 0;
  corrector = 0;
  return port.isOpen();
}

void Uart::close() {
  if (port.isOpen()) {
    port.close();
  }
}

bool Uart::openSaveFile(const QString& save_file_path) {
  num_rx_bytes = 0;
  raw_data_file.setFileName(save_file_path);
  return raw_data_file.open(QIODevice::ReadWrite);
}

void Uart::closeSaveFile() {
  raw_data_file.close();
}

bool Uart::write(const QByteArray& data) {
  if (!port.isOpen()) {
    emit error(QString("Нет соединения по %1").arg(uart_name));
    return false;
  }

  CRC16 crc16;

  for (int i = 0; i < data.size(); i++)
    crc16.feed(data[i]);

  unsigned short crc = crc16.result();

  QByteArray tx_buffer;
  tx_buffer.reserve(data.size() * 2);

  tx_buffer.push_back(_BOF_);

  auto convertor = [](unsigned char byte, QByteArray& buffer) {
    if (byte == Uart::_BOF_ || byte == Uart::_EOF_ || byte == Uart::_CE__) {
      buffer.push_back(Uart::_CE__);
      buffer.push_back(byte ^ Uart::_XC__);
    } else
      buffer.push_back(byte);
  };

  for (int i = 0; i < data.size(); i++)
    convertor(data[i], tx_buffer);

  convertor(crc & 0x00FF, tx_buffer);
  convertor((crc & 0xFF00) >> 8, tx_buffer);
  tx_buffer.push_back(_EOF_);

  qint64 num_write = port.write((char*)tx_buffer.data(), tx_buffer.size());
  //  if (REPEATER_FLAG) {
  //    gui::UartLogBox::instance()->output(data, uart_name + " ");
  //  } else {
  //    if (uart_name == "UART0") {
  //      gui::UartLogBox::instance()->output(data, "UART0 ");
  //    }
  //  }
  return num_write == tx_buffer.size();
}
/*
 *
bool Uart::write(const std::initializer_list<quint8>& data) {
  if (!port.isOpen()) {
    emit error(QString("Нет соединения по %1").arg(uart_name));
    return false;
  }

  CRC16 crc16;

  for (auto byte : data)
    crc16.feed(byte);

  unsigned short crc = crc16.result();

  QByteArray tx_buffer;
  tx_buffer.reserve(data.size() * 2);

  tx_buffer.push_back(_BOF_);

  auto convertor = [](unsigned char byte, QByteArray& buffer) {
    if (byte == Uart::_BOF_ || byte == Uart::_EOF_ || byte == Uart::_CE__) {
      buffer.push_back(Uart::_CE__);
      buffer.push_back(byte ^ Uart::_XC__);
    } else
      buffer.push_back(byte);
  };

  for (auto byte : data)
    convertor(byte, tx_buffer);

  convertor(crc & 0x00FF, tx_buffer);
  convertor((crc & 0xFF00) >> 8, tx_buffer);
  tx_buffer.push_back(_EOF_);

  qint64 num_write = port.write((char*)tx_buffer.data(), tx_buffer.size());
  if (uart_name == "UART0") {
    QByteArray ss;
    ss.resize(data.size());
    std::copy(data.begin(), data.end(), ss.begin());

    if (REPEATER_FLAG) {
      gui::UartLogBox::instance()->output(ss, uart_name + " ");
    } else {
      if (uart_name == "UART0") {
        gui::UartLogBox::instance()->output(ss, "UART0 ");
      }
    }
  }
  return num_write == tx_buffer.size();
}
*/

void Uart::read_from_uart() {
  while (!port.atEnd()) {
    QByteArray bytes = port.readAll();
    if (raw_data_file.isOpen()) {
      num_rx_bytes += bytes.size();
      raw_data_file.write(bytes);
      emit rxBytes(num_rx_bytes);
      continue;
    }
    if (bytes.isEmpty()) {
      emit error(QString("Ошибка чтения данных из %1.").arg(uart_name));
      return;
    }

    for (int i = 0; i < bytes.size(); i++) {
      unsigned char byte = bytes.at(i) ^ corrector;
      if (corrector) {
        rx_buffer[index++] = byte;
        corrector = 0;
        continue;
      }

      if (byte == _EOF_) {
        if (index >= 2) {
          CRC16 crc16;
          for (int j = 0; j < index - 2; j++) {
            crc16.feed(rx_buffer[j]);
          }

          unsigned short crc = rx_buffer[index - 1];
          crc <<= 8;
          crc |= (unsigned char)(rx_buffer[index - 2]);
          if (crc16.result() == crc) {
            auto re = rx_buffer.mid(0, index - 2);
            //            if (REPEATER_FLAG) {
            //              gui::UartLogBox::instance()->input(re, uart_name + "
            //              ");
            //            } else {
            //              if (uart_name == "UART0") {
            //                gui::UartLogBox::instance()->input(re, uart_name +
            //                " ");
            //              }
            //            }
            emit packetReceived(re);
          } else
            emit error(QString("В принятых данных в %1 не совпадает CRC.")
                           .arg(uart_name));
        } else
          emit error(QString("Размер принятой в %1 команды меньше минимального "
                             "(Мин = BOF CRC1 CRC2 EOF).")
                         .arg(uart_name));

        index = 0;
        continue;
      }
      if (byte == _BOF_) {
        if (index)
          emit error(QString("В %1 принят BOF до EOF.").arg(uart_name));
        corrector = index = 0;
        continue;
      }
      if (byte == _CE__) {
        corrector = _XC__;
        continue;
      }
      rx_buffer[index++] = byte;
      if (index >= rx_buffer.size())
        emit error(QString("В %1 принята команда, превышающая максимально "
                           "допустимый размер %2.")
                       .arg(uart_name)
                       .arg(MAX_RX_BUFFER_SIZE));
    }
  }
}
