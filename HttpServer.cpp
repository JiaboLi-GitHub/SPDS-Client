#include "HttpServer.h"
#include "JsonServer.h"
#include "TcpSocket.h"
#include "ServerConfig.h"
#include "Detection.h"

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
    request->setUrl(QUrl(URL));    
    request->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    connect(nam, &QNetworkAccessManager::finished, this, &HttpServer::resPost);
}

//检测请求
void HttpServer::post(SPDOnceData data)
{
	QByteArray byteArray = JsonServer::toHTTPQByteArray(data);
	QNetworkReply* reply = nam->post(*request, byteArray);
}

//检测完成后调用的槽函数
void HttpServer::resPost(QNetworkReply* reply)
{
	QByteArray byteArray = reply->readAll();
	SPDOnceData data = JsonServer::toSPDOnceData(byteArray);

    if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toQByteArray(data);
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            emit setStatus(data.result);
        }
    }
}