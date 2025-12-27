#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QSerialPort>
#include <QLabel>
#include <QVBoxLayout>

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

    void InitMainWindow();

    void InitSerialPort();

    void InitSendDataBtn();

    ~MainWindow();

signals:
    void ReceiveData();

 public slots:
     void UpdateData();
     void SendData();

private:
    Ui::MainWindow* ui;
    QSerialPort* serialPort;
    QVBoxLayout* mainLayout;
};
#endif // MAINWINDOW_H
