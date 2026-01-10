#include "ChartManager.h"

ChartManager::ChartManager(QObject* parent)
    : QObject{parent}
{
    initChart();
}

void ChartManager::initChart()
{
    chart = new QChart();
    chart -> legend() -> setVisible(false);

    chart->setBackgroundRoundness(0);
    chart->setTheme(QChart::ChartThemeLight);

    lineSeries = new QLineSeries(chart);
    lineSeries -> setPointsVisible(true);
    lineSeries -> setPointLabelsVisible(true);
    lineSeries -> setPointLabelsFormat("@yPoint");
    lineSeries -> setPointLabelsFont(QFont("Arial", 10));
    lineSeries -> setPointLabelsColor(Qt::black);
    lineSeries -> setPointLabelsClipping(false);

    axisX = new QDateTimeAxis(chart);
    axisY = new QValueAxis(chart);
    axisX -> setFormat("hh:mm:ss");
    axisX -> setTitleText("时间");
    axisY -> setRange(0, 255);
    axisY -> setTitleText("值");

    axisY->setMin(0 - 15);
    axisY->setMax(255 + 15);

    chart -> addSeries(lineSeries);
    chart -> addAxis(axisX, Qt::AlignBottom);
    chart -> addAxis(axisY, Qt::AlignLeft);
    lineSeries -> attachAxis(axisX);
    lineSeries -> attachAxis(axisY);

    chart->setMargins(QMargins(30, 30, 30, 30));
}

QChart* ChartManager::getChart() const
{
    return chart;
}

void ChartManager::addDataPoint(const QDateTime& timestamp, int value)
{
    dataPoints.append(QPointF(timestamp.toMSecsSinceEpoch(), value));

    while (dataPoints.size() > MAX_DATA_POINTS) {
        dataPoints.removeFirst();
    }

    lineSeries -> clear();
    for (const QPointF& point : dataPoints) {
        lineSeries -> append(point);
    }

    if (!dataPoints.isEmpty()) {
        qreal minX = dataPoints.first().x();
        qreal maxX = dataPoints.last().x();

        axisX -> setRange(QDateTime::fromMSecsSinceEpoch(minX), QDateTime::fromMSecsSinceEpoch(maxX + 1000));
    }
}

ChartManager::~ChartManager()
{
    delete chart;
}
