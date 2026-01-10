#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent)
    : QSerialPort(parent)
{
    initSerialPort();
}

void SerialPortManager::initSerialPort()
{
    QString portName = "/dev/ttyS3";
    setPortName(portName);
    qDebug() << "连接固定串口通道:" << portName;

    if(open(QIODevice::ReadWrite)) {
        setBaudRate(115200);
        setDataBits(QSerialPort::Data8);
        setStopBits(QSerialPort::OneStop);
        setParity(QSerialPort::NoParity);
        setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "串口打开成功，参数：波特率115200，8N1，权限可读可写。";
    }
    else{
        qWarning() << "无法打开串口！" << "错误：" << errorString();
    }
}

SerialPortManager::~SerialPortManager()
{
    if (isOpen()) {
        close();
    }
}
