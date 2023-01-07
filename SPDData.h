#pragma once
#include<qdatetime.h>
#include<qimage.h>

class SPDData
{
public:
	SPDData();
	~SPDData();

public:
	static enum Result
	{
		Correct,			//0正常
		LeftLeaning,		//1左倾
		RightLeaning		//2右倾
	};

public:
	QDateTime dateTime;		//时间日期
	QImage image;			//监测图片
	Result result;			//监测结果
};

