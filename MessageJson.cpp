#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>

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
    verificationData_json.insert("RequestType", TcpData::RequestType::LogIn);
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
    verificationData_json.insert("RequestType", TcpData::RequestType::Enroll);
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
