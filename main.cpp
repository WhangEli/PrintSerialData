/**
 * @file main.cpp
 * @brief 串口数据接收显示应用程序入口文件
 * 
 * 该文件包含了QT串口数据接收显示应用程序的主函数，
 * 负责创建主窗口、初始化串口通信，并启动应用程序事件循环。
 * 
 * 主要功能：
 * 1. 创建数据显示屏主窗口
 * 2. 程序自动初始化串口通信
 * 3. 串口数据自动接收和显示
 * 4. 显示格式："接收的数据：数据内容"
 */

#include "mainwindow.h"

#include <QApplication>

/**
 * @brief 应用程序主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 应用程序退出码
 * 
 * 主函数的主要流程：
 * 1. 创建QApplication对象
 * 2. 创建串口数据接收显示主窗口
 * 3. 显示主窗口（串口初始化在构造函数中自动完成）
 * 4. 启动应用程序事件循环
 * 
 * 注意：串口初始化、数据接收和显示逻辑都在MainWindow类中自动处理，
 * 主函数只需要创建窗口并启动事件循环即可。
 */
int main(int argc, char *argv[])
{
    // 创建QT应用程序对象
    QApplication a(argc, argv);
    
    // 创建串口数据接收显示主窗口
    // 串口初始化会在MainWindow构造函数中自动完成
    MainWindow w;
    
    // 显示主窗口
    w.show();
    
    // 启动应用程序事件循环，等待用户交互
    return a.exec();
}
