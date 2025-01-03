#include "serialprotocoldebugger.h"
#include "ui_serialprotocoldebugger.h"
#include <QMessageBox>
#include <QDebug>

SerialProtocolDebugger::SerialProtocolDebugger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialProtocolDebugger)
{
    ui->setupUi(this);
    refreshCom();

    connect(&_port, &QSerialPort::errorOccurred, this, &SerialProtocolDebugger::slotError);
    connect(&_port, &QSerialPort::readyRead, this, &SerialProtocolDebugger::slotRead, Qt::AutoConnection);
    connect(ui->bt_openclose, &QPushButton::clicked, this, &SerialProtocolDebugger::slotBtOpenClose);
}

SerialProtocolDebugger::~SerialProtocolDebugger()
{
    delete ui;
}

void SerialProtocolDebugger::refreshCom()
{
    ui->cb_port->clear();
    QStringList _strNames;
    QStringList _strDescription;
    QStringList viewTest;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        _strDescription.push_back(info.description());
    }
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        _strNames.push_back(info.portName());
    }
    for (int i = 0; i < _strNames.size(); i++) {
        viewTest.append(_strNames[i] + " " + _strDescription[i]);
    }

    ui->cb_port->addItems(viewTest);
    for (int i = 0; i < ui->cb_port->count(); i++) {
        ui->cb_port->setItemData(i, _strNames[i]);
    }
}

bool SerialProtocolDebugger::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_UNUSED(result);
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_DEVICECHANGE) {
        refreshCom();
    }
    return false;
}

void SerialProtocolDebugger::slotError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError)
        return;
    QMessageBox::critical(this, "critical", "error occur"
                          + ui->cb_port->currentData().toString()
                          + _port.errorString());
}

void SerialProtocolDebugger::slotBtOpenClose()
{
    if (!_port.isOpen()) {
        _port.setPortName(ui->cb_port->currentData().toString());
        _port.setBaudRate(ui->cb_baudrate->currentText().toInt());
        _port.setParity(QSerialPort::NoParity);
        _port.setStopBits(QSerialPort::OneStop);
        _port.setDataBits(QSerialPort::Data8);
        _port.setReadBufferSize(10 * 1024);
        if (_port.open(QIODevice::ReadWrite)) {
            ui->bt_openclose->setText(QString("Close"));
            _tim = new QTimer();
            _tim->setInterval(1000);
            connect(_tim, &QTimer::timeout, this, &SerialProtocolDebugger::onTimeout);
            _tim->start();
        } else {
            QMessageBox::critical(this, "critical", "Open Failed!!");
        }
    } else {
        ui->bt_openclose->setText(QString("Open"));
        _port.close();
        _tim->stop();
    }
}

void SerialProtocolDebugger::slotRead()
{
    if (_port.bytesAvailable() <= 0) return;
    QByteArray byteData = _port.readAll();

    switch (ui->cb_protocol->currentIndex()) {
    case RAW:
        // qDebug() << "rcv lenth: " << byteData.length();
        break;
    case GNSS:
        handle_gnss(byteData);
        break;
    default: break;
    }
}

void SerialProtocolDebugger::onTimeout()
{

}

void SerialProtocolDebugger::handle_gnss(QByteArray &array)
{
    char *p = array.data();
    int len = array.length();
    static rtcm_t msg;

    for (int i = 0; i < len; i++) {
        int res = input_rtcm3(&msg, p[i]);
        switch (res) {
        case RTCM3_FRAME_INCOMPLETE:
        case RTMC3_FRAME_BAD_CRC:
            break;
        case RTCM3_FRAME_OK:
            decode_rtcm3(&msg);
            if (msg.msg_number == 999) {
                qDebug() << "id: " << msg.msg_number << " sub: " << getbitu(msg.buff,36,8);
            }
            qDebug() << "tow: " << msg.tow;
            break;
        }
    }
}
