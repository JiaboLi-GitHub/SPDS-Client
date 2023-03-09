#pragma once
#include <qobject.h>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<qstring.h>

class HttpServer :
    public QObject
{
public:
    HttpServer();
    ~HttpServer();
    void init();
    void post(QByteArray byteArray);

public slots:
    void resPost(QNetworkReply* reply);

public:
    QNetworkRequest *request;
    QNetworkAccessManager *nam;
    static const QString URL;
};