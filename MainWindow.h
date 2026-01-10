#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Core/ChartManager.h"
#include "Core/SerialPortManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initMainWindow();
    void initDataDisplay();
    void initSendDataBtn();
    void initCloseBtn();

public slots:
    void sendData();
    void updateData();

private:
    Ui::MainWindow* ui;
    ChartManager* chartManager;
    SerialPortManager* serialPortManager;
};
#endif // MAINWINDOW_H
