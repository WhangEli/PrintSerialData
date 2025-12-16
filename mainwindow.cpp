/**
 * @file mainwindow.cpp
 * @brief MainWindow类的实现文件
 * 
 * 该文件包含了串口数据接收和显示的具体实现，
 * 包括串口初始化、数据接收、定时刷新和界面显示等功能。
 * 
 * 主要功能：
 * 1. 自动检测并打开串口设备（USB-UART）
 * 2. 接收串口数据并实时显示
 * 3. 支持自定义刷新频率
 * 4. 显示格式："接收的消息：数据内容"
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/**
 * @brief 构造函数
 * @param parent 父窗口指针
 * 
 * 初始化成员变量，创建定时器和串口对象，
 * 设置默认刷新频率为1秒，初始化串口并启动定时器。
 * 串口初始化包括自动检测和打开USB-UART设备。
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , refreshRate(1000)  // 默认1秒刷新一次
    , serialPort(nullptr) // 串口对象初始化为空
{
    ui->setupUi(this);
    
    // 创建定时器对象，用于控制显示刷新频率
    refreshTimer = new QTimer(this);
    
    // 连接定时器的timeout信号到刷新处理槽函数
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshTimeout);
    
    // 初始化串口通信
    initSerialPort();
    
    // 启动定时器，按照设定的频率开始刷新
    refreshTimer->start(refreshRate);
    
    // 设置窗口标题
    setWindowTitle("串口数据接收显示");
/**
 * @brief 轮询检查串口数据
 * 
 * 定期主动检查串口是否有数据可读，不依赖readyRead信号。
 * 解决readyRead信号在某些硬件环境下不触发的问题。
 */
void MainWindow::pollSerialData()
{
    if (serialPort && serialPort->isOpen() && serialPort->bytesAvailable() > 0) {
        QByteArray data = serialPort->readAll();
        
        if (!data.isEmpty()) {
            QString receivedData = QString::fromUtf8(data).trimmed();
            
            if (!receivedData.isEmpty()) {
                qDebug() << "轮询接收到串口数据：" << receivedData;
                
                // 更新标签数据
                updateData("接收的消息", receivedData);
                
                // 立即更新显示
                if (ui && ui->dataDisplayLabel) {
                    QString displayText = QString("接收的消息：%1").arg(receivedData);
                    ui->dataDisplayLabel->setText(displayText);
                }
            }
        }
    }
}

/**
 * @brief 析构函数
 * 
 * 释放UI对象占用的内存资源
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 设置显示刷新频率
 * @param msec 刷新间隔时间（毫秒）
 * 
 * 动态修改显示刷新频率，会先停止当前定时器，
 * 然后以新的频率重新启动定时器。
 * 
 * 注意：如果定时器当前未激活，则只更新频率值，不重启定时器
 */
void MainWindow::setRefreshRate(int msec)
{
    refreshRate = msec;
    if (refreshTimer->isActive()) {
        refreshTimer->stop();
        refreshTimer->start(refreshRate);
    }
}

/**
 * @brief 添加一个标签
 * @param tagName 标签名称
 * 
 * 向数据映射中添加一个新的标签，初始数据为空字符串
 */
void MainWindow::addTag(const QString& tagName)
{
    dataMap[tagName] = "";
}

/**
 * @brief 批量设置标签
 * @param tagNames 标签名称列表
 * 
 * 一次性设置多个标签，会清除之前的所有标签和数据
 */
void MainWindow::setTags(const QStringList& tagNames)
{
    dataMap.clear();  // 清除现有标签和数据
    for (const QString& tagName : tagNames) {
        dataMap[tagName] = "";  // 添加新标签，初始值为空字符串
    }
}

/**
 * @brief 更新指定标签的数据
 * @param tagName 标签名称
 * @param value 数据内容（可以是数值、字符串等各种类型）
 * 
 * 将指定标签的数据更新为给定值，数据会保存在内存中，
 * 等待下次定时器刷新时显示到界面上。
 * 
 * 支持各种数据类型：
 * - 数值："1", "25", "100"
 * - 字符串："正常", "异常", "运行中"
 * - 带单位："25°C", "100Pa", "50%"
 * - 状态信息："开启", "关闭", "故障"
 */
void MainWindow::updateData(const QString& tagName, const QString& value)
{
    if (dataMap.contains(tagName)) {
        dataMap[tagName] = value;
    }
}

/**
 * @brief 定时器超时处理槽函数
 * 
 * 当定时器超时时自动调用，负责更新界面显示内容。
 * 显示"接收的消息：数据内容"，保持上次接收到的数据显示，
 * 直到收到新数据才更新显示内容。
 */
void MainWindow::onRefreshTimeout()
{
    if (!dataMap.isEmpty()) {
        QString displayText;
        
        // 构建显示文本，只处理"接收的消息"标签
        auto it = dataMap.find("接收的消息");
        if (it != dataMap.end()) {
            QString tagName = it.key();
            QString value = it.value();
            
            // 显示格式：标签名 + 数据内容（如果数据为空，只显示标签名）
            if (!value.isEmpty()) {
                displayText = QString("%1：%2").arg(tagName).arg(value);
            } else {
                displayText = QString("%1：").arg(tagName);
            }
            
            // 显示数据
            ui->dataDisplayLabel->setText(displayText);
        }
    } else {
        // 没有标签时显示为空
        ui->dataDisplayLabel->setText("");
    }
}

/**
 * @brief 初始化串口
 * 
 * 自动检测系统中可用的串口设备，优先选择USB-UART设备（通常是/dev/ttyUSB0），
 * 设置串口参数：波特率1500000，数据位8，停止位1，无校验位，
 * 并连接数据接收信号槽。
 */
void MainWindow::initSerialPort()
{
    // 创建串口对象
    serialPort = new QSerialPort(this);
    
    // 获取系统中所有可用的串口信息
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    
    if (availablePorts.isEmpty()) {
        qWarning() << "未找到可用的串口设备！";
        return;
    }
    
    // 优先选择USB-UART设备（通常是ttyUSB0）
    QString portName;
    for (const QSerialPortInfo& portInfo : availablePorts) {
        if (portInfo.portName().contains("USB", Qt::CaseInsensitive) || 
            portInfo.description().contains("USB", Qt::CaseInsensitive) ||
            portInfo.description().contains("UART", Qt::CaseInsensitive)) {
            portName = portInfo.portName();
            qDebug() << "找到USB-UART串口设备：" << portName << "描述：" << portInfo.description();
            break;
        }
    }
    
    // 如果没有找到USB设备，使用第一个可用串口
    if (portName.isEmpty()) {
        portName = availablePorts.first().portName();
        qDebug() << "使用第一个可用串口：" << portName << "描述：" << availablePorts.first().description();
    }
    
    // 设置串口名称
    serialPort->setPortName(portName);
    
    // 设置串口参数
    if (serialPort->open(QIODevice::ReadOnly)) {
        // 设置波特率1500000（使用自定义波特率）
        serialPort->setBaudRate(1500000);
        
        // 设置数据位8位
        serialPort->setDataBits(QSerialPort::Data8);
        
        // 设置停止位1位
        serialPort->setStopBits(QSerialPort::OneStop);
        
        // 设置无校验位
        serialPort->setParity(QSerialPort::NoParity);
        
        // 设置无流控制
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        
    // 连接串口数据接收信号槽（备用，轮询方式将作为主要读取方式）
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);
    
    // 启动轮询定时器，定期检查串口数据（100ms检查一次）
    QTimer* pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &MainWindow::pollSerialData);
    pollTimer->start(100);
    
    qDebug() << "串口" << portName << "打开成功，参数：波特率1500000，8N1，轮询模式启动";
        
        // 初始化标签
        dataMap["接收的消息"] = "";  // 添加接收消息标签
        
    } else {
        qWarning() << "无法打开串口" << portName << "错误：" << serialPort->errorString();
        
        // 即使串口打开失败，也初始化标签用于显示
        dataMap["接收的消息"] = "";
    }
}

/**
 * @brief 串口数据接收槽函数
 * 
 * 当串口接收到新数据时自动调用，负责读取并处理接收到的数据。
 * 接收到新数据后立即更新界面显示，保持数据显示直到收到下一个新数据。
 */
void MainWindow::onSerialDataReceived()
{
    if (serialPort && serialPort->isOpen()) {
        QByteArray data = serialPort->readAll();
        
        if (!data.isEmpty()) {
            QString receivedData = QString::fromUtf8(data).trimmed();
            
            if (!receivedData.isEmpty()) {
                qDebug() << "接收到串口数据：" << receivedData;
                
                // 更新标签数据
                updateData("接收的消息", receivedData);
                
                // 立即更新显示，不需要等待定时器
                if (ui && ui->dataDisplayLabel) {
                    QString displayText = QString("接收的消息：%1").arg(receivedData);
                    ui->dataDisplayLabel->setText(displayText);
                }
            }
        }
    }
}
