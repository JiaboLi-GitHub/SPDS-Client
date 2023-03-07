#pragma once
#include <qobject.h>
#include <qdatetime.h>

class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	//��������
	enum RequestType
	{
		VerificationCode,	 //��������֤��
		Enroll_Request,		 //ע��
		LogIn_Request,		 //��¼
		Detection_Save_Request, //������������
		Detection_Read_Request, //������������
		Auto_Login_Request, //�Զ���¼����
		Quit_Request //ע����¼
	};

	//��Ӧ����
	enum ResponseType
	{
		Enroll_Response,	//ע��
		LogIn_Response,		//��¼
		Detection_Read_Response, //������������
		Auto_Login_Response //�Զ���¼��Ӧ
	};

	//ע����Ӧ����
	enum Enroll_Response
	{
		Enroll_Correct,		//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Enroll_error		//δ֪����
	};

	//��¼��Ӧ����
	enum Login_Response
	{
		Login_Correct,		//��¼�ɹ�
		Account_Error,		//�˺Ŵ���
		Login_error			//δ֪����
	};

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //�Զ���¼�ɹ�
		Auto_Login_Error       //�Զ���¼ʧ��
	};
};

class SPDDataPerDay : public QObject
{
	Q_OBJECT

public:
	SPDDataPerDay(QDate date, double accuracy, qint32 totalCount, qint32 normal,
		qint32 head, qint32 front, qint32 back, qint32 left, qint32 right) :
		date(date), accuracy(accuracy), normal(normal), head(head),
		front(front), back(back), left(left), right(right) {}
	~SPDDataPerDay() = default;

private:
	QDate date;
	double accuracy;
	qint32 totalCount;
	qint32 normal, head, front, back, left, right;
};

class SPDData :QObject
{
	Q_OBJECT
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
	SPDData(QDate date, Detection_Result result) :
		date(date), result(result) {}
	~SPDData() = default;

public:
	QDate date;		//����
	Detection_Result result;//�����
};