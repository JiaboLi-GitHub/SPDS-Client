#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>

/*************************************************
Description: ������֤������ֽ���
      Input: mailAddress=��Ҫ��֤�������ַ
     Return: ��֤������ֽ���
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
Description: ���ɵ�¼��Ϣ�ֽ���
      Input: mailAddress=������Ϣ
             password=������Ϣ
     Return: ��¼��Ϣ���ֽ���
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
Description: ����ע����Ϣ�ֽ���
      Input: mailAddress=������Ϣ
             password=������Ϣ
             userName=�û�����Ϣ
             code=��֤����Ϣ
     Return: ע����Ϣ���ֽ���
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
