#pragma once
#include<qtcpsocket.h>
#include<qbytearray.h>


class TcpSocket
{
public:
	TcpSocket();
	~TcpSocket();

public:
	static bool connectToHost(QString ip, unsigned int port);
	static long long write(const QByteArray& byteArray);

public:
	static QTcpSocket *tcpSocket;
};

