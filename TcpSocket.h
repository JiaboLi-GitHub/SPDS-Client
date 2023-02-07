#pragma once
#include<qtcpsocket.h>
#include<qbytearray.h>
#include<qthread.h>


class TcpSocket:
	public QThread
{
public:
	TcpSocket();
	~TcpSocket();

public:
	static bool connectToHost(QString ip, unsigned int port);
	static qint64 write(const QByteArray& byteArray);
	static bool isConnected();
	static bool isReceived();
	static QByteArray read();

public:
	static QTcpSocket *tcpSocket;
};

