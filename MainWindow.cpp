#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    InitMainWindow();

    InitSerialPort();

    InitSendDataBtn();
}

void MainWindow::InitMainWindow()
{
    setWindowTitle("串口数据接收显示");

    mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addStretch(2);
    mainLayout->addWidget(ui->dataLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch(1);
}

void MainWindow::SendData()
{
    if (serialPort->isOpen() && serialPort->isWritable()) {
        QByteArray response = "1";
        qint64 bytesWritten = serialPort->write(response);
        if (bytesWritten == -1) {
            qWarning() << "发送响应失败：" << serialPort->errorString();
        } else {
            qDebug() << "成功发送响应：" << response;
            serialPort->flush();  // 确保数据立即发送
        }
    }
}

void MainWindow::UpdateData()
{
    QByteArray data = serialPort->readAll();
    QString receivedData = QString::fromUtf8(data).trimmed();
    if (!receivedData.isEmpty()) {
        ui -> dataLabel -> setText(QString("接收的数据：%1").arg(receivedData));
        qDebug() << "接收到串口数据：" << receivedData;
    }
}

void MainWindow::InitSerialPort()
{
    serialPort = new QSerialPort(this);
    QString portName = "/dev/ttyS3";
    serialPort -> setPortName(portName);
    qDebug() << "连接固定串口通道:" << portName;
    if(serialPort -> open(QIODevice::ReadWrite)) {
        serialPort -> setBaudRate(115200);
        serialPort -> setDataBits(QSerialPort::Data8);
        serialPort -> setStopBits(QSerialPort::OneStop);
        serialPort -> setParity(QSerialPort::NoParity);
        serialPort -> setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "串口打开成功，参数：波特率115200，8N1，权限可读可写。";
    }
    else{
        qWarning() << "无法打开串口！" << "错误：" << serialPort -> errorString();
    }

    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::UpdateData);

}

void MainWindow::InitSendDataBtn()
{
    QPushButton* sendDataBtn = new QPushButton("发送", this);
    mainLayout->addWidget(sendDataBtn, 0, Qt::AlignCenter);
    mainLayout->addStretch(6);

    connect(sendDataBtn, &QPushButton::clicked, this, &MainWindow::SendData);
}

MainWindow::~MainWindow()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;
}
