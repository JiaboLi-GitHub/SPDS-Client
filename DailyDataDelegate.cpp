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

    //������ͼ
    QPieSeries* series = new QPieSeries();
    series->append("����", 20);
    series->append("��ͷ", 40);
    series->append("ǰ��", 60);
    series->append("����", 60);
    series->append("����", 60);
    series->append("����", 60);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(u8"2022/2/14");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //���Ʊ�ͼ
    painter->save();
    chartView->render(painter, option.rect);
    painter->restore();
}

QSize DailyDataDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(200, 200);
}