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
    //�����
	enum Detection_Result
	{
		Normal, //����
		Head,   //��ͷ
		Front,  //ǰ��
		Back,   //����
		Left,   //����
		Right   //����
	};

public:
	QDate date;		//����
	Detection_Result result;//�����
};

