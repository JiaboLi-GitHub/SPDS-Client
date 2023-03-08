#include "MessageJson.h"
#include "TcpData.h"

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
      Input: loginData=登录信息
     Return: 登录信息的字节流
*************************************************/
QByteArray MessageJson::loginDataToQByteArray(LoginData loginData)
{
    QString mailAddress = loginData.mailAddress; 
    QString password = loginData.password;
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::LogIn_Request);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    data_json.insert("password", password);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray loginData_byteArray = document.toBinaryData();
    return loginData_byteArray;
}

/*************************************************
Description: 生成自动登录信息字节流
      Input: token=本地保存的用户token
     Return: 自动登录信息的字节流
*************************************************/
QByteArray MessageJson::autoLoginDataToQByteArray(QString token)
{
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::LogIn_Request);
    QJsonObject data_json;
    data_json.insert("token", token);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray autoLoginData_byteArray = document.toBinaryData();
    return autoLoginData_byteArray;
}

/*************************************************
Description: 生成注册信息字节流
      Input: enrolldata=注册信息
     Return: 注册信息的字节流
*************************************************/
QByteArray MessageJson::enrollToQByteArray(EnrollData enrolldata)
{
    QString mailAddress = enrolldata.mailAddress;
    QString password = enrolldata.password;
    QString userName = enrolldata.userName; 
    QString code = enrolldata.code;
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::Enroll_Request);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    data_json.insert("password", password);
    data_json.insert("userName", userName);
    data_json.insert("code", code);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray enroll_byteArray = document.toBinaryData();
    return enroll_byteArray;
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
    case TcpData::Detection_Read_Response: responseStr = Detection_Read_Response_Str; break;
    default:throw QException();
        break;
    }
    return responseStr;
}
