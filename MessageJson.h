#pragma once
#include <qbytearray.h>
#include <qjsonobject.h>
#include "TcpData.h"

class MessageJson
{
public:
	static QByteArray verificationDataToQByteArray(QString id);
	static QByteArray loginDataToQByteArray(LoginData loginData);
	static QByteArray autoLoginDataToQByteArray(QString token);
	static QByteArray enrollToQByteArray(EnrollData enrolldata);
	
public:
	static TcpData::RequestType getRequestType(QByteArray& byteArray);
	static QString getMailAddress(QByteArray& byteArray);

	static EnrollData toEnrollData(QByteArray& byteArray);
	static LoginData toLogInData(QByteArray& byteArray);
	static SPDData toSPDOnceData(QByteArray& byteArray);

	static QByteArray toQByteArray(EnrollData data);
	static QByteArray toQByteArray(LoginData data);
	static QByteArray toQByteArray(QVector<SPDData> dataList);

private:
	static QStringList getResponseStr(TcpData::ResponseType type);

private:
	static const QStringList Enroll_Response_Str;
	static const QStringList Login_Response_Str;
};

