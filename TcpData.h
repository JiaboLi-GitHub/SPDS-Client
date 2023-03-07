#pragma once
#include <qobject.h>
#include <qdatetime.h>

class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	//请求类型
	enum RequestType
	{
		VerificationCode,	 //请求发送验证码
		Enroll_Request,		 //注册
		LogIn_Request,		 //登录
		Detection_Save_Request, //发送样本数据
		Detection_Read_Request, //请求样本数据
		Auto_Login_Request, //自动登录请求
		Quit_Request //注销登录
	};

	//响应类型
	enum ResponseType
	{
		Enroll_Response,	//注册
		LogIn_Response,		//登录
		Detection_Read_Response, //返回样本数据
		Auto_Login_Response //自动登录响应
	};

	//注册响应类型
	enum Enroll_Response
	{
		Enroll_Correct,		//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Enroll_error		//未知错误
	};

	//登录响应类型
	enum Login_Response
	{
		Login_Correct,		//登录成功
		Account_Error,		//账号错误
		Login_error			//未知错误
	};

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //自动登录成功
		Auto_Login_Error       //自动登录失败
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
	SPDData(QDate date, Detection_Result result) :
		date(date), result(result) {}
	~SPDData() = default;

public:
	QDate date;		//日期
	Detection_Result result;//监测结果
};