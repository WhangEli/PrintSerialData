#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QSerialPort>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QVector>
#include <QSizePolicy>
#include <QDateTime>
#include <QGraphicsTextItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
    class QChartView;
class QChart;
class QLineSeries;
class QDateTimeAxis;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

        QT_CHARTS_USE_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void InitMainWindow();
    void InitSerialPort();
    void InitSendDataBtn();
    void InitCloseBtn();
    void InitChart();
    void UpdateChart();

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
    QHBoxLayout* topLayout;
    QChartView* chartView;
    QLineSeries* lineSeries;
    QDateTimeAxis* axisX;
    QValueAxis* axisY;
    QVector<QPointF> dataPoints;
    QVector<QGraphicsTextItem*> dataLabels;
};
#endif // MAINWINDOW_H
