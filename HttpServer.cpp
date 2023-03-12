#include "HttpServer.h"
#include "JsonServer.h"
#include "TcpSocket.h"
#include "ServerConfig.h"
#include "Detection.h"
#include<qfile.h>

const QString HttpServer::URL= "http://127.0.0.1:8989/image";

HttpServer::HttpServer()
{
	request = new QNetworkRequest();
	nam = new QNetworkAccessManager();
	init();
}

HttpServer::~HttpServer()
{
	delete request;
	delete nam;
}

void HttpServer::init()
{
    request->setUrl(URL);    
    request->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    connect(nam, &QNetworkAccessManager::finished, this, &HttpServer::resPost);
}

//检测请求
void HttpServer::post(SPDOnceData data)
{
    QByteArray imageBytes = JsonServer::toHTTPQByteArray(data);
   // qDebug() << byteArray;
	QNetworkReply* reply = nam->post(*request, imageBytes);
}

//检测完成后调用的槽函数
void HttpServer::resPost(QNetworkReply* reply)
{
	QByteArray byteArray = reply->readAll();
    qDebug() << byteArray;
	SPDOnceData data = JsonServer::toSPDOnceData(byteArray);
    qDebug() <<data.date.toString() << " " << data.result;
    emit setStatus(data.result);

    /*if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toQByteArray(data);
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            emit setStatus(data.result);
            qDebug() << data.result;
        }
    }*/
}