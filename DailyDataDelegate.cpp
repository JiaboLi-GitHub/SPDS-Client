#include<qpieseries.h>
#include<qchart.h>
#include<qchartview.h>
#include "DailyDataDelegate.h"

DailyDataDelegate::DailyDataDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

DailyDataDelegate::~DailyDataDelegate()
{}

void DailyDataDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    QT_CHARTS_USE_NAMESPACE

    //´´½¨±ýÍ¼
    QPieSeries* series = new QPieSeries();
    series->append("First", 20);
    series->append("Second", 40);
    series->append("Third", 60);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Sample pie chart");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //»æÖÆ±ýÍ¼
    painter->save();
    chartView->render(painter, option.rect);
    painter->restore();
}

QSize DailyDataDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(200, 200);
}