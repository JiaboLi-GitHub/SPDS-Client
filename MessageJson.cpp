#include "MessageJson.h"
#include<qstring.h>
#include<qmap.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qexception.h>
#include<qstringlist.h>

const QStringList MessageJson::Enroll_Response_Str = { "mailAddress" };
const QStringList MessageJson::Login_Response_Str = { "userName","mailAddress","password","code" };

/*************************************************
Description: 生成验证邮箱的字节流
      Input: mailAddress=需要验证的邮箱地址
     Return: 验证邮箱的字节流
*************************************************/
QByteArray MessageJson::verificationDataToQByteArray(QString mailAddress)
{
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::VerificationCode);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray requestCode_byteArray = document.toBinaryData();
    return requestCode_byteArray;
}

/*************************************************
Description: 生成登录信息字节流
      Input: mailAddress=邮箱信息
             password=密码信息
     Return: 登录信息的字节流
*************************************************/
QByteArray MessageJson::loginDataToQByteArray(QString mailAddress, QString password)
{
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::LogIn_Request);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    data_json.insert("password", password);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray requestCode_byteArray = document.toBinaryData();
    return requestCode_byteArray;
}

/*************************************************
Description: 生成注册信息字节流
      Input: mailAddress=邮箱信息
             password=密码信息
             userName=用户名信息
             code=验证码信息
     Return: 注册信息的字节流
*************************************************/
QByteArray MessageJson::enrollToQByteArray(QString mailAddress, QString password, QString userName, QString code)
{
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::Enroll_Request);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    data_json.insert("password", password);
    data_json.insert("userName", userName);
    data_json.insert("code", code);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray requestCode_byteArray = document.toBinaryData();
    return requestCode_byteArray;
}

/*************************************************
Description: 解析请求类型
	  Input: 来自客户端的请求字节数组
	 Return: 请求类型
*************************************************/
TcpData::ResponseType MessageJson::getResponseType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject response_json = document.object();
	int typeId = response_json["ResponseType"].toInt();
	TcpData::ResponseType type = TcpData::ResponseType(typeId);
	return type;
}

/*************************************************
Description: 解析请求数据
	  Input: 来自客户端的请求字节数组
	 Return: 解析完成的数据
*************************************************/
QMap<QString, QString> MessageJson::getResponseData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject response_json = document.object();
	QJsonObject data_json = response_json["data"].toObject();
	int typeId = response_json["ResponseType"].toInt();
	TcpData::ResponseType type = TcpData::ResponseType(typeId);
	QStringList keyList = getResponseStr(type);

	QMap<QString, QString> data;
	for (auto& key : keyList)
	{
		data[key] = data_json[key].toString();
	}
	return data;
}

/*************************************************
Description: 根据请求类型获取数据格式
      Input: 请求类型
     Return: 数据格式
*************************************************/
QStringList MessageJson::getResponseStr(TcpData::ResponseType type)
{
    QStringList responseStr;
    switch (type)
    {
    case TcpData::Enroll_Response:	responseStr = Enroll_Response_Str; break;
    case TcpData::LogIn_Response:	responseStr = Login_Response_Str; break;
    default:throw QException();
        break;
    }
    return responseStr;
}
