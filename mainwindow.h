#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief 串口数据接收显示主窗口类
 * 
 * 该类实现了一个串口数据接收和显示界面，主要功能包括：
 * 1. 连接固定串口设备/dev/ttyS3
 * 2. 接收串口数据并实时显示
 * 3. 支持自定义刷新频率
 * 4. 显示格式："接收的数据：数据内容"
 * 
 * 串口参数：
 * - 波特率：115200
 * - 数据位：8
 * - 停止位：1
 * - 校验位：无
 * 
 * 使用方式：
 * 1. 创建MainWindow对象
 * 2. 程序会自动初始化串口
 * 3. 接收到的数据会自动显示在界面上
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~MainWindow();

    /**
     * @brief 设置显示刷新频率
     * @param msec 刷新间隔时间（毫秒）
     * 
     * 例如：
     * - 1000 表示1秒刷新一次
     * - 500  表示0.5秒刷新一次
     * - 2000 表示2秒刷新一次
     */
    void setRefreshRate(int msec);
    
    /**
     * @brief 添加一个标签
     * @param tagName 标签名称（如"A"、"温度"等）
     * 
     * 可以多次调用此函数添加多个标签
     */
    void addTag(const QString& tagName);
    
    /**
     * @brief 批量设置标签
     * @param tagNames 标签名称列表
     * 
     * 一次性设置多个标签，会清除之前的标签
     */
    void setTags(const QStringList& tagNames);
    
    /**
     * @brief 更新指定标签的数据
     * @param tagName 标签名称
     * @param value 数据内容（可以是数值、字符串等各种类型）
     * 
     * 数据格式示例：
     * - updateData("A", "1") 将显示 "A：1"
     * - updateData("状态", "正常") 将显示 "状态：正常"
     * - updateData("温度", "25°C") 将显示 "温度：25°C"
     * - updateData("接收的数据", "Hello World") 将显示 "接收的数据：Hello World"
     */
    void updateData(const QString& tagName, const QString& value);

private slots:
    /**
     * @brief 定时器超时处理槽函数
     * 
     * 定时器超时处理，但只在串口接收到新数据时更新界面显示
     * 其他情况下不更新界面，避免不必要的刷新
     */
    void onRefreshTimeout();
    
    /**
     * @brief 串口数据接收槽函数
     * 
     * 当串口有数据可读时自动调用，从/dev/ttyS3读取串口数据并更新显示
     */
    void onSerialDataReceived();

private:
    /**
     * @brief 初始化串口
     * 
     * 连接固定串口设备/dev/ttyS3，设置串口参数
     * 波特率：115200，数据位：8，停止位：1，校验位：无
     */
    void initSerialPort();
    
    Ui::MainWindow *ui;           // UI界面指针
    QTimer* refreshTimer;         // 刷新定时器指针
    QMap<QString, QString> dataMap;     // 存储各标签的数据（标签名 -> 数据内容）
    int refreshRate;              // 刷新频率（毫秒）
    QSerialPort* serialPort;      // 串口对象指针
};
#endif // MAINWINDOW_H
