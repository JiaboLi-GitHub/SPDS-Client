#pragma once
#include<qdatetime.h>
#include<qimage.h>

class SPDData:QObject
{
	Q_OBJECT

public:
	SPDData();
	~SPDData();

public:
    //检测结果
	enum Detection_Result
	{
		Normal, //正常
		Head,   //托头
		Front,  //前倾
		Back,   //后倾
		Left,   //左倾
		Right   //右倾
	};

public:
	QDate date;		//日期
	Detection_Result result;//监测结果
};

