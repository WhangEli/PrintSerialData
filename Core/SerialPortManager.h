#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QSerialPort>
#include <QDebug>

class SerialPortManager : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    void initSerialPort();
};

#endif // SERIALPORTMANAGER_H
