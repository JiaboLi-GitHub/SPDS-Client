#include "HttpServer.h"

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

void HttpServer::post(QByteArray byteArray)
{
	QNetworkReply* reply = nam->post(*request, byteArray);
}

void HttpServer::resPost(QNetworkReply* reply)
{
	QByteArray byteArray = reply->readAll();

}