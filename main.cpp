/**
 * @file main.cpp
 * @brief 应用程序入口文件
 * 
 * 该文件包含了QT应用程序的主函数，
 * 负责创建主窗口、设置标签、模拟数据更新逻辑，
 * 并启动应用程序事件循环。
 */

#include "mainwindow.h"

#include <QApplication>
#include <QTimer>

/**
 * @brief 应用程序主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 应用程序退出码
 * 
 * 主函数的主要流程：
 * 1. 创建QApplication对象
 * 2. 创建数据显示屏主窗口
 * 3. 设置标签（可以自定义标签名和数量）
 * 4. 显示主窗口
 * 5. 设置测试数据更新逻辑（模拟外部数据源）
 * 6. 启动应用程序事件循环
 */
int main(int argc, char *argv[])
{
    // 创建QT应用程序对象
    QApplication a(argc, argv);
    
    // 创建数据显示屏主窗口
    MainWindow w;
    
    // 设置标签 - 可以自定义标签名和数量
    // 方法1：逐个添加标签
    w.addTag("A");
    w.addTag("B");
    w.addTag("温度");
    w.addTag("状态");
    w.addTag("消息");
    
    // 方法2：批量设置标签（会清除之前的标签）
    // w.setTags(QStringList() << "X" << "Y" << "Z" << "压力");
    
    // 显示主窗口
    w.show();
    
    // 测试示例：模拟数据更新
    QTimer testTimer;           // 创建测试定时器
    int counter = 0;            // 计数器，用于生成测试数据
    
    /**
     * @brief 测试数据更新槽函数
     * 
     * 每500毫秒生成新的测试数据，分别更新不同标签的数据
     * 通过调用MainWindow的updateData()方法更新指定标签的数据
     * 
     * 现在支持各种数据类型：
     * - 数值数据："1", "25", "100"
     * - 字符串数据："正常", "异常", "运行中"
     * - 带单位数据："25°C", "100Pa", "50%"
     * - 状态信息："开启", "关闭", "故障"
     * - 消息文本："Hello World", "系统启动完成"
     */
    QObject::connect(&testTimer, &QTimer::timeout, [&]() {
        counter++;  // 计数器递增
        
        // 更新不同标签的各种类型数据
        w.updateData("A", QString::number(counter));           // 数值："A：1"
        w.updateData("B", QString::number(counter * 2));         // 数值："B：2"
        w.updateData("温度", QString("%1°C").arg(20 + counter % 10)); // 带单位："温度：20°C"
        w.updateData("状态", (counter % 3 == 0) ? "正常" : "异常");   // 字符串："状态：正常/异常"
        w.updateData("消息", (counter % 4 == 0) ? "系统启动完成" : "运行中"); // 消息文本
    });
    
    // 每500ms更新一次数据（注意：这是数据更新频率，不是显示刷新频率）
    testTimer.start(500);
    
    /**
     * @brief 刷新频率设置示例
     * 
     * 可以修改显示刷新频率，比如设置为2秒刷新一次
     * 默认刷新频率为1秒（1000毫秒）
     * 
     * 注意区分：
     * - 数据更新频率：这里是500ms，由testTimer控制
     * - 显示刷新频率：默认1000ms，可通过setRefreshRate()修改
     */
    // w.setRefreshRate(2000);  // 取消注释可设置为2秒刷新一次
    
    // 启动应用程序事件循环，等待用户交互
    return a.exec();
}
