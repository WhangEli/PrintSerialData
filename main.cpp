#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    qputenv("QT_QPA_EGLFS_ROTATION", "90");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.showFullScreen();
    return a.exec();
}
