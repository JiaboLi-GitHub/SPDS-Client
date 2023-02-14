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

    QChart* mainChart, * secondChart, * thirdChart;
    mainChart = initQLineChart();
    secondChart = initQLineChart();
    thirdChart = initQLineChart();

    mainWidget->setChart(mainChart);
    secondChart->resize(secondWidget->size());
    secondWidget->setChart(secondChart);
    thirdChart->resize(thirdWidget->size());
    thirdWidget->setChart(thirdChart);


    initListWidget(
        listWidget, 9,
        initQPieChartView("2023/2/14", 60, 70, 80, 90, 100, 1015),
        initQPieChartView("2023/2/13", 15, 959, 192, 5652, 626, 12),
        initQPieChartView("2023/2/12", 15, 15, 35, 13, 45, 05),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25)
    );
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

QChartView* Visualization::initQPieChartView(QString date,
    qint32 gesture1, qint32 gesture2, qint32 gesture3,
    qint32 gesture4, qint32 gesture5, qint32 gesture6)
{
    //创建饼图
    QPieSeries* series = new QPieSeries();
    series->append("正常", gesture1);
    series->append("托头", gesture2);
    series->append("前倾", gesture3);
    series->append("后倾", gesture4);
    series->append("左倾", gesture5);
    series->append("右倾", gesture6);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(date);
    chart->setMargins(QMargins(-10, -10, -10, -10));
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
        item->setSizeHint(QSize(200, 180));
        listWidget->addItem(item);
        listWidget->setItemWidget(item, view);
        listWidget->setSpacing(5);
    }
    va_end(args);
}