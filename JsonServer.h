#pragma once
#include<qbytearray.h>
#include<qjsonobject.h>
#include<qsqlquery.h>
#include"TcpData.h"
#include"SPDData.h"

class JsonServer
{
public:
	static TcpData::RequestType getRequestType(QByteArray& byteArray);
	
	static EnrollData toEnrollData(QByteArray& byteArray);
	static LoginData toLogInData(QByteArray& byteArray);
	static QVector<SPDData> toSPDDataList(QByteArray& byteArray);

	static QByteArray toQByteArray(CodeData data);
	static QByteArray toQByteArray(EnrollData data);
	static QByteArray toQByteArray(LoginData data);
	static QByteArray toQByteArray(SPDOnceData data);
};

