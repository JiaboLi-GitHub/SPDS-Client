#pragma once
#include "ModuleLoader.h"
#include "SPDData.h"

#include <qimage.h>

class Module
	: public QObject
{
	Q_OBJECT

public:
	Module(QObject* parent);
	~Module();

public:
	cv::Mat qim2mat(const QImage& qimg);

public slots:
	void forward(const QImage& captureImage);

signals:
	void setStatus(SPDData::Detection_Result status);

private:
	//依次为batchsize、通道数、图像高度、图像宽度
	static std::vector<int64_t> sizes;
	SPDData::Detection_Result prevStatus;
};
