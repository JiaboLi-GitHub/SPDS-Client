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

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //自动登录成功
		Auto_Login_Error       //自动登录失败
	};
};

class CodeData
{
public:
	CodeData(QString mailAddress):mailAddress(mailAddress){}
	~CodeData() {};

public:
	QString mailAddress;
};

class EnrollData
{
public:
	EnrollData() {};
	EnrollData(QString userName, QString mailAddress, QString password, int code) : 
		userName(userName), mailAddress(mailAddress), password(password), 
		code(code) {}
	~EnrollData() {}

public:
	//注册响应类型
	enum Enroll_Response
	{
		Enroll_Correct,		//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Enroll_error		//未知错误
	};
	QString userName;
	QString mailAddress;
	QString password;
	int code;
	int enroll_response;
};

class LoginData
{
public:
	LoginData() {};
	LoginData(QString mailAddress, QString password) 
		: mailAddress(mailAddress), password(password){}
	~LoginData() {};

public:
	//登录响应类型
	enum Login_Response
	{
		Login_Correct,		//登录成功
		Account_Error,		//账号错误（账号不存在）
		Password_Error,		//密码错误
		Login_Error			//未知错误
	};
	QString userName;
	QString mailAddress;
	QString password;
	QString token;
	int login_response;
};

class QuitData
{
public:
	QuitData() {};
	~QuitData() {};
};

class AutoLogInData 
{
public:
	AutoLogInData() {};
	~AutoLogInData() {};
};