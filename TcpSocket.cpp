#include "TcpSocket.h"

#include<qtcpsocket.h>

QTcpSocket* TcpSocket::tcpSocket = new QTcpSocket();


TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

/*************************************************
Description: 连接服务端
	  Input: ip=服务端IP地址
			 port=服务端的端口
	 Return: 是否连接成功
*************************************************/
bool TcpSocket::connectToHost(QString ip, unsigned int port)
{
	tcpSocket->connectToHost(ip, port);
	return tcpSocket->waitForConnected();
}

/*************************************************
Description: 检查服务端连接
	 Return: 是否连接成功
*************************************************/
bool TcpSocket::isConnected()
{
	return tcpSocket->waitForConnected();
}

/*************************************************
Description: 等待服务器回应
	 Return: 是否接收到服务器回应
*************************************************/
bool TcpSocket::isReceived()
{
	return tcpSocket->waitForReadyRead();
}

/*************************************************
Description: 读取服务器的回应
	 Return: 收到的字节流
*************************************************/
QByteArray TcpSocket::read()
{
	return tcpSocket->readAll();
}

/*************************************************
Description: 向服务端发送信息
	  Input: byteArray=待发送的字节流
	 Return: 写入的字节数
*************************************************/
qint64 TcpSocket::write(const QByteArray& byteArray)
{
	return tcpSocket->write(byteArray);
}
