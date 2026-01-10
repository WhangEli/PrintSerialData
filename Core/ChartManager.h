#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QtCharts>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>

    class ChartManager : public QObject
{
    Q_OBJECT
public:
    explicit ChartManager(QObject* parent = nullptr);
    ~ChartManager();

    void initChart();

    QChart* getChart() const;

    void addDataPoint(const QDateTime& timestamp, int value);

private:
    QChart* chart;
    QLineSeries* lineSeries;
    QDateTimeAxis* axisX;
    QValueAxis* axisY;
    QList<QPointF> dataPoints;
    static const int MAX_DATA_POINTS = 10;
};
#endif // CHARTMANAGER_H
