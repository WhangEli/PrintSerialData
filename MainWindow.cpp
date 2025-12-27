#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    InitMainWindow();
    InitSerialPort();
    InitSendDataBtn();
    InitCloseBtn();
    InitChart();

    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::UpdateData);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::SendData);
}

void MainWindow::InitMainWindow()
{
    setWindowTitle("串口数据接收显示");

    mainLayout = new QVBoxLayout(ui -> centralwidget);

    topLayout = new QHBoxLayout();
    topLayout -> addWidget(ui -> dataLabel);
    topLayout -> addStretch();

    mainLayout -> addLayout(topLayout);
}

void MainWindow::SendData()
{
    if (serialPort -> isOpen() && serialPort -> isWritable()) {
        QByteArray response = "1";
        qint64 bytesWritten = serialPort -> write(response);
        if (bytesWritten == -1) {
            qWarning() << "发送响应失败：" << serialPort -> errorString();
        } else {
            qDebug() << "成功发送响应：" << response;
            serialPort -> flush();  // 确保数据立即发送
        }
    }
}

void MainWindow::UpdateData()
{
    QByteArray data = serialPort->readAll();
    QString receivedData = QString::fromUtf8(data).trimmed();
    if (!receivedData.isEmpty()) {
        ui -> dataLabel -> setText(QString("接收的数据：%1").arg(receivedData));
        qDebug() << "接收到串口数据：" << receivedData;

        for (int i = 0; i < data.size(); ++i) {
            unsigned char byte = data[i];
            qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
            dataPoints.append(QPointF(timestamp, byte));

            if (dataPoints.size() > 10) {
                dataPoints.removeFirst();
            }
        }
        UpdateChart();
    }
}

void MainWindow::InitSerialPort()
{
    serialPort = new QSerialPort(this);
    QString portName = "/dev/ttyS3";
    serialPort -> setPortName(portName);
    qDebug() << "连接固定串口通道:" << portName;
    if(serialPort -> open(QIODevice::ReadWrite)) {
        serialPort -> setBaudRate(115200);
        serialPort -> setDataBits(QSerialPort::Data8);
        serialPort -> setStopBits(QSerialPort::OneStop);
        serialPort -> setParity(QSerialPort::NoParity);
        serialPort -> setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "串口打开成功，参数：波特率115200，8N1，权限可读可写。";
    }
    else{
        qWarning() << "无法打开串口！" << "错误：" << serialPort -> errorString();
    }
}

void MainWindow::InitSendDataBtn()
{
    QPushButton* sendDataBtn = new QPushButton("发送", this);
    topLayout -> addWidget(sendDataBtn);

    connect(sendDataBtn, &QPushButton::clicked, this, &MainWindow::SendData);
}

void MainWindow::InitCloseBtn()
{
    QPushButton* closeBtn = new QPushButton("关闭", this);
    topLayout -> addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::InitChart()
{
    lineSeries = new QLineSeries();
    lineSeries -> setName("串口数据");
    lineSeries -> setPointsVisible(true);

    axisX = new QDateTimeAxis();
    axisX -> setTitleText("时间");
    axisX -> setFormat("hh:mm:ss.zzz");

    axisY = new QValueAxis();
    axisY -> setTitleText("ASCII值");
    axisY -> setRange(0, 255);

    QChart* chart = new QChart();
    chart -> addSeries(lineSeries);
    chart -> addAxis(axisX, Qt::AlignBottom);
    chart -> addAxis(axisY, Qt::AlignLeft);
    lineSeries -> attachAxis(axisX);
    lineSeries -> attachAxis(axisY);
    chart -> legend() -> hide();

    chartView = new QChartView(chart);
    chartView -> setRenderHint(QPainter::Antialiasing);
    chartView -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout -> addWidget(chartView);
}

void MainWindow::UpdateChart()
{
    lineSeries -> clear();

    for (const QPointF& point : dataPoints) {
        lineSeries -> append(point);
    }

    qDeleteAll(dataLabels);
    dataLabels.clear();

    if (!dataPoints.isEmpty()) {
        QDateTime minTime = QDateTime::fromMSecsSinceEpoch(dataPoints.first().x());
        QDateTime maxTime = QDateTime::fromMSecsSinceEpoch(dataPoints.last().x());
        axisX -> setRange(minTime, maxTime);

        double minValue = dataPoints.first().y();
        double maxValue = dataPoints.first().y();
        for (const QPointF& point : dataPoints) {
            if (point.y() < minValue) minValue = point.y();
            if (point.y() > maxValue) maxValue = point.y();
        }
        double padding = (maxValue - minValue) * 0.1;
        if (padding < 1) padding = 1;
        axisY -> setRange(minValue - padding, maxValue + padding);

        QChart* chart = chartView -> chart();
        for (const QPointF& point : dataPoints) {
            QGraphicsTextItem* label = new QGraphicsTextItem(chart);
            label -> setPlainText(QString::number((int)point.y()));
            label -> setDefaultTextColor(Qt::black);
            label -> setFont(QFont("Arial", 8));

            QPointF chartPos = chart -> mapToPosition(point);
            label -> setPos(chartPos.x() - 10, chartPos.y() - 20);

            chart -> scene() -> addItem(label);
            dataLabels.append(label);
        }
    }
}

MainWindow::~MainWindow()
{
    qDeleteAll(dataLabels);
    dataLabels.clear();

    if (serialPort && serialPort -> isOpen()) {
        serialPort -> close();
    }
    delete ui;
}
