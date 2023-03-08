#include "TcpSocket.h"

#include<qtcpsocket.h>

QTcpSocket* TcpSocket::tcpSocket = new QTcpSocket();


TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

/*************************************************
Description: ���ӷ����
	  Input: ip=�����IP��ַ
			 port=����˵Ķ˿�
	 Return: �Ƿ����ӳɹ�
*************************************************/
bool TcpSocket::connectToHost(QString ip, unsigned int port)
{
	tcpSocket->connectToHost(ip, port);
	return tcpSocket->waitForConnected();
}

/*************************************************
Description: �����������
	 Return: �Ƿ����ӳɹ�
*************************************************/
bool TcpSocket::isConnected()
{
	return tcpSocket->waitForConnected();
}

/*************************************************
Description: �ȴ���������Ӧ
	 Return: �Ƿ���յ���������Ӧ
*************************************************/
bool TcpSocket::isReceived()
{
	return tcpSocket->waitForReadyRead();
}

/*************************************************
Description: ��ȡ�������Ļ�Ӧ
	 Return: �յ����ֽ���
*************************************************/
QByteArray TcpSocket::read()
{
	return tcpSocket->readAll();
}

/*************************************************
Description: �����˷�����Ϣ
	  Input: byteArray=�����͵��ֽ���
	 Return: д����ֽ���
*************************************************/
qint64 TcpSocket::write(const QByteArray& byteArray)
{
	return tcpSocket->write(byteArray);
}
