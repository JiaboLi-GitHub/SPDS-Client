#pragma once

#include <qlistwidget.h>
#include <qchartview.h>

QT_CHARTS_USE_NAMESPACE

#include "ui_Visualization.h"

class Visualization : public QWidget
{
	Q_OBJECT

public:
	Visualization(QWidget *parent);
	~Visualization();

private:
	Ui::VisualizationClass ui;
	qint32 availableItem;
	QtCharts::QChartView* mainWidget,*secondWidget,*thirdWidget;
	QListWidget* listWidget;
	QChartView* initQPieChartView(qint32 x);
	QChart* initQLineChart();
	void initListWidget(QListWidget* listWidget, qint32 count, ...);
};
