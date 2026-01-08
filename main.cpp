#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);  // 设置窗口始终在最前面
    qputenv("QT_QPA_EGLFS_ROTATION", "90");  // 90°或270°旋转:ml-citation{ref="1" data="citationList"}   //设置窗口旋转90°，横屏显示。

    w.showFullScreen();
    return a.exec();
}
