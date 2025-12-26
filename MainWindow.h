#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>

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

    void InitSendDataBtn();

    void InitSerialPort();

    ~MainWindow();

signals:
    void ReceiveData();
    void ClickBtn();

// public slots:
//     void UpdateData();
//     void SendData();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
