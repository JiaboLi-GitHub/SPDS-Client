#include "JsonServer.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qmap.h>
#include<qstringlist.h>
#include"TcpData.h"
#include<qdebug.h>

#include"SPDData.h"
#include <QtCore/qbuffer.h>

/*************************************************
Description: 解析请求类型
	  Input: 
	 Return: 
*************************************************/
TcpData::RequestType JsonServer::getRequestType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	int typeId = request_json["RequestType"].toInt();
	TcpData::RequestType type = TcpData::RequestType(typeId);
	return type;
}

//注册
EnrollData JsonServer::toEnrollData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	EnrollData enrollData;
	enrollData.enroll_response = data_json["Enroll_Response"].toInt();
	return enrollData;
}

//登录
LoginData JsonServer::toLogInData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	LoginData loginData;
	loginData.login_response = data_json["Login_Response"].toInt();
	loginData.mailAddress = data_json["mailAddress"].toString();
	loginData.userName = data_json["userName"].toString();
	loginData.token = data_json["token"].toString();
	return loginData;
}

//5天数据
QVector<SPDData> JsonServer::toSPDDataList(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonArray dataList_json = request_json["data"].toArray();

	QVector<SPDData> spdDataList;
	for (int i = 0; i < dataList_json.size(); i++)
	{
		auto data_json = dataList_json.at(i).toObject();
		SPDData spdData;
		spdData.date = QDate::fromString(data_json["date"].toString(), "yyyy-MM-dd");
		spdData.accuracy = data_json["accuracy"].toDouble();
		spdData.number = data_json["number"].toInt();
		spdData.normal = data_json["normal"].toInt();
		spdData.head = data_json["head"].toInt();
		spdData.front = data_json["front"].toInt();
		spdData.back = data_json["back"].toInt();
		spdData.left = data_json["left"].toInt();
		spdData.right = data_json["right"].toInt();
		spdDataList.append(spdData);
	}
	return spdDataList;
}

SPDOnceData JsonServer::toSPDOnceData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject http_json = document.object();

	SPDOnceData data;
	data.date = QDate::fromString(http_json["time"].toString(),"yyyy-MM-dd");
	data.result = (SPDOnceData::Detection_Result)http_json["data"].toInt();
	return data;
}

//验证码
QByteArray JsonServer::toQByteArray(CodeData data)
{
	QJsonObject requestData_json, data_json;
	data_json.insert("mailAddress", data.mailAddress);
	requestData_json.insert("RequestType", TcpData::VerificationCode);
	requestData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(requestData_json);
	return document.toBinaryData();
}

//注册
QByteArray JsonServer::toQByteArray(EnrollData data)
{
	QJsonObject requestData_json, data_json;
	data_json.insert("userName", data.userName);
	data_json.insert("mailAddress", data.mailAddress);
	data_json.insert("password", data.password);
	data_json.insert("code", data.code);
	requestData_json.insert("RequestType", TcpData::Enroll_Request);
	requestData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(requestData_json);
	return document.toBinaryData();
}

//登录
QByteArray JsonServer::toQByteArray(LoginData data)
{
	QJsonObject requestData_json, data_json;
	data_json.insert("mailAddress", data.mailAddress);
	data_json.insert("password", data.password);
	requestData_json.insert("RequestType", TcpData::LogIn_Request);
	requestData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(requestData_json);
	return document.toBinaryData();
}

//一次检测数据
QByteArray JsonServer::toQByteArray(SPDOnceData data)
{
	QJsonObject reqData_json;
	QJsonArray dataList_json;
	reqData_json.insert("date", data.date.toString());
	reqData_json.insert("Detection_Result", data.result);
	reqData_json.insert("RequestType", TcpData::Detection_Save_Request);
	reqData_json.insert("data", dataList_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(reqData_json);
	return document.toBinaryData();
}

//请求检测数据
QByteArray JsonServer::toQByteArray(GetSPDData data)
{
	QJsonObject requestData_json, data_json;
	data_json.insert("token", data.token);
	requestData_json.insert("RequestType", TcpData::Detection_Read_Request);
	requestData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(requestData_json);
	return document.toBinaryData();
}


QString  toBase64(QImage image)
{
	QByteArray ba;
	QBuffer buf(&ba);
	image.save(&buf, "png");
	return ba.toBase64();
}

//用于HTTP请求的数据
QByteArray JsonServer::toHTTPQByteArray(SPDOnceData data)
{
	QJsonObject http_json;
	http_json.insert("time", data.date.toString("yyyy-MM-dd"));
	http_json.insert("image", toBase64(data.image));
	QJsonDocument document = QJsonDocument::QJsonDocument(http_json);
	return document.toBinaryData();
}
