#ifndef SERIALPROTOCOLDEBUGGER_H
#define SERIALPROTOCOLDEBUGGER_H

#include <QWidget>
#include <QSerialPort>
#include <qt_windows.h>
#include <QStringList>
#include <QSerialPortInfo>
#include <QTimer>

#include <rtklib.h>

namespace Ui {
class SerialProtocolDebugger;
}

class SerialProtocolDebugger : public QWidget
{
    Q_OBJECT

public:
    explicit SerialProtocolDebugger(QWidget *parent = nullptr);
    ~SerialProtocolDebugger();

public slots:
    void slotBtOpenClose();
    void slotRead();
    void slotError(QSerialPort::SerialPortError error);

    void onTimeout();

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

private:
    enum protocol_select {
        RAW = 0,
        GNSS,
    };

    Ui::SerialProtocolDebugger *ui;

    QTimer     *_tim;
    QString     _portname;
    QString     _portbaud;
    QSerialPort _port;

    void refreshCom();

    void handle_gnss(QByteArray &array);
};

#endif // SERIALPROTOCOLDEBUGGER_H
