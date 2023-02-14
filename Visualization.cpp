#include <qlineseries.h>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include "Visualization.h"
#include "DailyDataDelegate.h"

Visualization::Visualization(QWidget* parent)
	: QWidget(parent)
{
    ui.setupUi(this);

    mainWidget = ui.mainWidget;
    secondWidget = ui.secondWidget;
    thirdWidget = ui.thirdWidget;
    listWidget = ui.listWidget;
    availableItem = 0;

    QChart* mainChart, *secondChart, *thirdChart;
    mainChart = initQLineChart();
    secondChart = initQLineChart();
    thirdChart = initQLineChart();

    mainWidget->setChart(mainChart);
    secondChart->resize(secondWidget->size());
    secondWidget->setChart(secondChart);
    thirdChart->resize(thirdWidget->size());
    thirdWidget->setChart(thirdChart);


    initListWidget(listWidget,4, initQPieChartView(1), initQPieChartView(2), initQPieChartView(2), initQPieChartView(2));
}

Visualization::~Visualization()
{
}

QChart* Visualization::initQLineChart()
{
    QLineSeries* series = new QLineSeries;
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);

    QChart* chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Simple line chart example");
    chart->createDefaultAxes();

    QValueAxis* axisY = new QValueAxis;
    axisY->setRange(0, 100);
    chart->setAxisY(axisY, series);

    return chart;
}

QChartView* Visualization::initQPieChartView(qint32 x)
{
    //´´½¨±ýÍ¼
    QPieSeries* series = new QPieSeries();
    series->append("First", x);
    series->append("Second", 1);
    series->append("Third", 1);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Sample pie chart");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void Visualization::initListWidget(QListWidget* listWidget, qint32 count, ...)
{
    va_list args;
    va_start(args, count);
    while (count--)
    {
        QChartView* view = va_arg(args, QChartView*);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(view->size());
        listWidget->addItem(item);
        listWidget->setItemWidget(item, view);
    }
    va_end(args);
}