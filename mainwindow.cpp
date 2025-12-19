#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(nullptr) // 串口对象初始化为空
{
    ui->setupUi(this);
    
    initSerialPort();
    
    // 设置窗口标题
    setWindowTitle("串口数据接收显示");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData(const QString& value)
{
    ui->dataDisplayLabel->setText(QString("接收的数据：%1").arg(value));
}

void MainWindow::initSerialPort()
{
    // 创建串口对象
    serialPort = new QSerialPort(this);
    
    // 设置固定串口设备路径
    QString portName = "/dev/ttyS3";
    serialPort->setPortName(portName);
    qDebug() << "使用固定串口设备：" << portName;
    
    // 设置串口参数
    if (serialPort->open(QIODevice::ReadOnly)) {
        // 设置波特率115200
        serialPort->setBaudRate(115200);
        
        // 设置数据位8位
        serialPort->setDataBits(QSerialPort::Data8);
        
        // 设置停止位1位
        serialPort->setStopBits(QSerialPort::OneStop);
        
        // 设置无校验位
        serialPort->setParity(QSerialPort::NoParity);
        
        // 设置无流控制
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        
        // 连接串口数据接收信号槽
        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);
        
        qDebug() << "串口" << portName << "打开成功，参数：波特率115200，8N1";
    }
    else {
            qWarning() << "无法打开串口" << portName << "错误：" << serialPort->errorString();
    }
}

void MainWindow::onSerialDataReceived()
{
    if (serialPort && serialPort->isOpen()) {
        QByteArray data = serialPort->readAll();
        
        if (!data.isEmpty()) {
            QString receivedData = QString::fromUtf8(data).trimmed();
            
            if (!receivedData.isEmpty()) {
                qDebug() << "接收到串口数据：" << receivedData;
                
                // 如果有换行符，只取最后一行（最新数据）
                QStringList lines = receivedData.split('\n');
                if (!lines.isEmpty()) {
                    QString latestData = lines.last().trimmed();
                    if (!latestData.isEmpty()) {
                        // 更新数据
                        updateData(latestData);
                    }
                }
            }
        }
    }
}
