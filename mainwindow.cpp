/**
 * @file mainwindow.cpp
 * @brief MainWindow类的实现文件
 * 
 * 该文件包含了支持多标签的数据显示屏的具体实现，
 * 包括标签管理、数值数据接收、定时刷新和界面显示等功能。
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief 构造函数
 * @param parent 父窗口指针
 * 
 * 初始化成员变量，创建定时器并连接信号槽，
 * 设置默认刷新频率为1秒，启动定时器。
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , refreshRate(1000)  // 默认1秒刷新一次
{
    ui->setupUi(this);
    
    // 创建定时器对象，用于控制显示刷新频率
    refreshTimer = new QTimer(this);
    
    // 连接定时器的timeout信号到刷新处理槽函数
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshTimeout);
    
    // 启动定时器，按照设定的频率开始刷新
    refreshTimer->start(refreshRate);
    
    // 设置窗口标题
    setWindowTitle("数据显示屏");
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
 * 显示所有标签的当前数据，然后清空所有数据。
 * 
 * 显示格式：每个标签占一行，格式为"标签名：数据内容"
 * 例如：
 * A：1
 * B：正常
 * 温度：25°C
 * 状态：运行中
 */
void MainWindow::onRefreshTimeout()
{
    if (!dataMap.isEmpty()) {
        QString displayText;
        
        // 构建显示文本，每个标签占一行
        for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
            QString tagName = it.key();
            QString value = it.value();
            if (!value.isEmpty()) {
                displayText += QString("%1：%2\n").arg(tagName).arg(value);
            }
        }
        
        // 显示数据（去除末尾换行符）
        if (!displayText.isEmpty()) {
            ui->dataDisplayLabel->setText(displayText.trimmed());
        } else {
            ui->dataDisplayLabel->setText("");
        }
        
        // 清空所有数据，确保下次刷新时显示为空
        for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
            it.value() = "";  // 清空数据内容
        }
    } else {
        // 没有标签时显示为空
        ui->dataDisplayLabel->setText("");
    }
}
