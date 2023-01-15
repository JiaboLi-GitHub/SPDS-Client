#pragma once
#include"qbytearray.h"
#include"TcpData.h"

class MessageJson
{
public:
	static QByteArray verificationDataToQByteArray(QString id);
	static QByteArray loginDataToQByteArray(QString mailAddress, QString password);
	static QByteArray enrollToQByteArray(QString mailAddress, QString password, QString userName, QString code);
};

