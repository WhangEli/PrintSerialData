#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartManager(new ChartManager(this))
    , serialPortManager(new SerialPortManager(this))
{
    ui -> setupUi(this);

    initMainWindow();
    initDataDisplay();
    initSendDataBtn();
    initCloseBtn();

    //使图表与UI中的chartView关联
    ui -> chartView -> setChart(chartManager -> getChart());
    ui -> chartView->setRenderHint(QPainter::Antialiasing);   //启用抗锯齿渲染,让图表的线条、文字、边缘都更平滑
}

void MainWindow::sendData()
{
    if (serialPortManager -> isOpen() && serialPortManager -> isWritable()) {
        QByteArray response = "1";
        qint64 bytesWritten = serialPortManager -> write(response);
        if (bytesWritten == -1) {
            qWarning() << "发送响应失败：" << serialPortManager -> errorString();
        }
        else {
            qDebug() << "成功发送响应：" << response;
            serialPortManager -> flush();  // 确保数据立即发送
        }
    }
}

void MainWindow::updateData()
{
    QByteArray data = serialPortManager -> readAll();
    if (!data.isEmpty()) {
        int value = static_cast<unsigned char>(data[0]);

        QString receivedData = QString::fromUtf8(data).trimmed();
        ui -> dataLabel -> setText(QString("接收的数据：%1 (ASCII: %2)").arg(receivedData).arg(value));
        qDebug() << "接收到串口数据：" << receivedData << "ASCII码：" << value;

        QDateTime timestamp = QDateTime::currentDateTime();
        chartManager->addDataPoint(timestamp, value);
    }
}

void MainWindow::initMainWindow()
{
    setWindowTitle("数据接收显示窗口");
}

void MainWindow::initDataDisplay()
{
    connect(serialPortManager, &SerialPortManager::readyRead, this, &MainWindow::updateData);
    connect(serialPortManager, &SerialPortManager::readyRead, this, &MainWindow::sendData);
}

void MainWindow::initSendDataBtn()
{
    connect(ui -> sendDataBtn, &QPushButton::clicked, this, &MainWindow::sendData);
}

void MainWindow::initCloseBtn()
{
    connect(ui -> closeBtn, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}
