#include "TcpSocket.h"
#include<qtcpsocket.h>

QTcpSocket* TcpSocket::tcpSocket = new QTcpSocket();


TcpSocket::TcpSocket()
{
		
}

TcpSocket::~TcpSocket()
{
}


bool TcpSocket::connectToHost(QString ip, unsigned int port)
{

	tcpSocket->connectToHost(ip, port);
	return tcpSocket->waitForConnected();
}

long long TcpSocket::write(const QByteArray& byteArray)
{
	return tcpSocket->write(byteArray);
}
