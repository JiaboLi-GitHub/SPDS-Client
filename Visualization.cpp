#include <qlineseries.h>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <qfontdatabase.h>

#include "Visualization.h"

Visualization::Visualization(QWidget* parent)
	:QWidget(parent)
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
	
	mainChart->resize(mainWidget->size());
	mainWidget->setChart(mainChart);
	secondChart->resize(secondWidget->size());
	secondWidget->setChart(secondChart);
	thirdChart->resize(thirdWidget->size());
	thirdWidget->setChart(thirdChart);

	int font_Id = QFontDatabase::addApplicationFont(":/SPDS_Client/resources/font/Alibaba_PuHuiTi_2.0_55_Regular_55_Regular.ttf");
	QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
	if (!font_list.isEmpty())
	{
		QFont f16(font_list[0],16), f12(font_list[0],12);
		mainChart->setTitleFont(f16);
		secondChart->setTitleFont(f12);
		thirdChart->setTitleFont(f12);
	}

    initListWidget(
        listWidget, 5,
        initQPieChartView("2023/2/14", 1080, 0, 0, 0, 0, 0),
        initQPieChartView("2023/2/13", 15, 2, 3, 11, 4, 12),
        initQPieChartView("2023/2/12", 15, 15, 35, 13, 45, 05),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        /*initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25),*/
        initQPieChartView("2023/2/11", 19, 12, 65, 95, 26, 25)
    );
}

Visualization::~Visualization()
{

}

QChart* Visualization::initQLineChart()
{
    QLineSeries* series = new QLineSeries(this);
    series->append(0, 80);
    series->append(1, 60);
    series->append(2, 40);
    series->append(3, 60);
    series->append(4, 25);
    series->append(5, 90);
    series->append(6, 100);

    QChart* chart = new QChart;
    chart->setParent(this);
    chart->addSeries(series);
    chart->setTitle(u8"坐姿正确率");
    chart->legend()->setVisible(false);
    chart->createDefaultAxes();

    QValueAxis* axisY = new QValueAxis(this);
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%d%");
    chart->setAxisY(axisY, series);

    QCategoryAxis* axisX = new QCategoryAxis(this);
    axisX->append("2022/01/01", 0);
    axisX->append("2022/01/02", 1);
    axisX->append("2022/01/03", 2);
    axisX->append("2022/01/04", 3);
    axisX->append("2022/01/05", 4);
    axisX->append("2022/01/06", 5);
    axisX->append("2022/01/07", 6);
    axisX->setRange(0, 6);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    chart->setAxisX(axisX, series);

    return chart;
}

QChartView* Visualization::initQPieChartView(QString date,
    qint32 pos0, qint32 pos1, qint32 pos2,
    qint32 pos3, qint32 pos4, qint32 pos5)
{
	QPieSlice* slice0, * slice1, * slice2, * slice3, * slice4, * slice5;
	QPieSeries* series = new QPieSeries(this);

	if (pos1 + pos2 + pos3 + pos4 + pos5 == 0)
	{
		slice0 = new QPieSlice(u8"正常", pos0);
		slice0->setLabel(QString("%1:%2").arg(slice0->label()).arg(slice0->value()));
		series->append(slice0);
	}
	else
	{
		if (pos1)
		{
			slice1 = new QPieSlice(u8"托头", pos1);
			slice1->setLabel(QString("%1:%2").arg(slice1->label()).arg(slice1->value()));
			series->append(slice1);
		}

		if (pos2)
		{
			slice2 = new QPieSlice(u8"前倾", pos2);
			slice2->setLabel(QString("%1:%2").arg(slice2->label()).arg(slice2->value()));
			series->append(slice2);
		}

		if (pos3)
		{
			slice3 = new QPieSlice(u8"后倾", pos3);
			slice3->setLabel(QString("%1:%2").arg(slice3->label()).arg(slice3->value()));
			series->append(slice3);
		}

		if (pos4)
		{
			slice4 = new QPieSlice(u8"左倾", pos4);
			slice4->setLabel(QString("%1:%2").arg(slice4->label()).arg(slice4->value()));
			series->append(slice4);
		}

		if (pos5)
		{
			slice5 = new QPieSlice(u8"右倾", pos5);
			slice5->setLabel(QString("%1:%2").arg(slice5->label()).arg(slice5->value()));
			series->append(slice5);
		}
	}

	//创建饼图
	series->setPieSize(0.7);
	series->setLabelsVisible(true);
	series->setUseOpenGL(true);
	//series->setHoleSize(0.5);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(date);
    chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
    chart->legend()->setVisible(false);
    chart->resize(QSize(200, 200));
    chart->setTitleFont(QFont(QString::fromUtf8("\351\230\277\351\207\214\345\267\264\345\267\264\346\231\256\346\203\240\344\275\223 2.0 55 Regular"),14));
    chart->setMargins(QMargins(0, 0, 0, 0));

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

void Visualization::initListWidget(QListWidget* listWidget, qint32 count, ...)
{
    listWidget->setSpacing(5);

    va_list args;
    va_start(args, count);
    while (count--)
    {
        QChartView* view = va_arg(args, QChartView*);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(300, listWidget->height()*7));
        listWidget->addItem(item);
        listWidget->setItemWidget(item, view);
    }
    va_end(args);
}