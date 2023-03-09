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

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //�Զ���¼�ɹ�
		Auto_Login_Error       //�Զ���¼ʧ��
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
	//ע����Ӧ����
	enum Enroll_Response
	{
		Enroll_Correct,		//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Enroll_error		//δ֪����
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
	//��¼��Ӧ����
	enum Login_Response
	{
		Login_Correct,		//��¼�ɹ�
		Account_Error,		//�˺Ŵ����˺Ų����ڣ�
		Password_Error,		//�������
		Login_Error			//δ֪����
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