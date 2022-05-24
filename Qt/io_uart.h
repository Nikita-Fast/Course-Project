#ifndef _IO_UART_H_
#define _IO_UART_H_

#include <initializer_list>

#include <qfile.h>
#include <qserialport.h>

namespace io {

class Uart final : public QObject {
  Q_OBJECT

 public:
  enum class UartNo { Uart0 = 0, Uart1 = 1 };

  Uart(QString Uart_Name, QObject* parent = Q_NULLPTR);
  ~Uart();

  bool isOpen() const;
  bool setBaudRate(qint32 rate);
  void setPortName(const QString& name);

  QString getPortName() const;

  bool open();
  void close();

  bool openSaveFile(const QString& save_file_path);
  void closeSaveFile();

  bool write(const QByteArray& data);
  bool write(const std::initializer_list<unsigned char>& data);

 private:
  static const qint32 MAX_RX_BUFFER_SIZE = 65536 * 100;
  static const quint8 _BOF_ = 0xC0;
  static const quint8 _EOF_ = 0xC1;
  static const quint8 _CE__ = 0x7D;
  static const quint8 _XC__ = 0x20;

  QByteArray rx_buffer;
  QString uart_name;
  QSerialPort port;
  qint32 index;
  quint8 corrector;
  quint64 num_rx_bytes;
  QFile raw_data_file;

 private slots:

  void read_from_uart();

 signals:

  void packetReceived(QByteArray);
  void error(QString);
  void rxBytes(quint64);
};

}  // namespace io

#endif  // !_IO_UART_H_
