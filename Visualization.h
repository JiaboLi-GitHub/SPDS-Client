#pragma once
#include "SPDData.h"

#include <qvector.h>
#include <qlistwidget.h>
#include <qchartview.h>

QT_CHARTS_USE_NAMESPACE

#include "ui_Visualization.h"

class Visualization : public QWidget
{
	Q_OBJECT

public:
	Visualization(QWidget* parent, QVector<SPDData> &dataList);
	~Visualization();

private:
	Ui::VisualizationClass ui;
	qint32 availableItem;
	QtCharts::QChartView* mainWidget, * secondWidget, * thirdWidget;
	QListWidget* listWidget;
	QChartView* initQPieChartView(QString date,
		qint32 gesture1, qint32 gesture2, qint32 gesture3,
		qint32 gesture4, qint32 gesture5, qint32 gesture6);
	QChart* initQLineChart();
	void initListWidget(QListWidget* listWidget, qint32 count, ...);
	void initListWidget(QListWidget* listWidget, QVector<QChartView*> &pieChartViewList);
	void topieChartViewList(QVector<SPDData> &dataList);
	QVector<SPDData> dataList;
	QVector<QChartView*> pieChartViewList;
};
