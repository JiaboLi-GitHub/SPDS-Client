#pragma once
#include <qbytearray.h>
#include <qjsonobject.h>
#include "TcpData.h"

class MessageJson
{
public:
	static QByteArray verificationDataToQByteArray(QString id);
	static QByteArray loginDataToQByteArray(QString mailAddress, QString password);
	static QByteArray autoLoginDataToQByteArray(QString token);
	static QByteArray enrollToQByteArray(QString mailAddress, QString password, QString userName, QString code);
	
public:
	static TcpData::ResponseType getResponseType(QByteArray& byteArray);
	static QMap<QString, QString> getResponseData(QByteArray& byteArray);

private:
	static QStringList getResponseStr(TcpData::ResponseType type);

private:
	static const QStringList Enroll_Response_Str;
	static const QStringList Login_Response_Str;
};

