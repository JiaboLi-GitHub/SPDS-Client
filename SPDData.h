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
		Correct,			//0����
		LeftLeaning,		//1����
		RightLeaning		//2����
	};

public:
	QDateTime dateTime;		//ʱ������
	QImage image;			//���ͼƬ
	Result result;			//�����
};

