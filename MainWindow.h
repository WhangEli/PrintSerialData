#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateData(const QString& value);

private slots:
    void onSerialDataReceived();
    void sendResponse();

private:
    void initSerialPort();

    Ui::MainWindow* ui;
    QSerialPort* serialPort;      // 串口对象指针
};
#endif // MAINWINDOW_H
