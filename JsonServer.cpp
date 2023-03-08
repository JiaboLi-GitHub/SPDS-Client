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

//验证码没有

EnrollData JsonServer::toEnrollData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	EnrollData enrollData;
	enrollData.enroll_response = data_json["Enroll_Response"].toInt();
	return enrollData;
}

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

QByteArray JsonServer::toQByteArray(SPDOnceData data)
{
	QJsonObject ResponseData_json;
	QJsonArray dataList_json;

	//差

	ResponseData_json.insert("RequestType", TcpData::Detection_Read_Response);
	ResponseData_json.insert("data", dataList_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(ResponseData_json);
	return document.toBinaryData();
}
