#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief 数据显示屏主窗口类
 * 
 * 该类实现了一个支持多标签的数据显示屏界面，主要功能包括：
 * 1. 支持自定义标签名和数量
 * 2. 接收数值数据并与标签名组合显示
 * 3. 按照设定的频率定时刷新显示
 * 4. 支持动态修改刷新频率
 * 
 * 使用方式：
 * 1. 创建MainWindow对象
 * 2. 通过addTag()或setTags()设置标签
 * 3. 调用updateData()更新指定标签的数值数据
 * 4. 可选：调用setRefreshRate()修改刷新频率
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
     * - updateData("消息", "Hello World") 将显示 "消息：Hello World"
     */
    void updateData(const QString& tagName, const QString& value);

private slots:
    /**
     * @brief 定时器超时处理槽函数
     * 
     * 当定时器超时时调用，负责更新界面显示内容
     * 显示所有标签的当前数值，然后清空数据
     */
    void onRefreshTimeout();

private:
    Ui::MainWindow *ui;           // UI界面指针
    QTimer* refreshTimer;         // 刷新定时器指针
    QMap<QString, QString> dataMap;     // 存储各标签的数据（标签名 -> 数据内容）
    int refreshRate;              // 刷新频率（毫秒）
};
#endif // MAINWINDOW_H
