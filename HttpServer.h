#pragma once
#include <qobject.h>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<qhttpmultipart.h>
#include<qnetworkaccessmanager.h>
#include<qnetworkreply.h>
#include<qstring.h>
#include"SPDData.h"

class HttpServer :
    public QObject
{
    Q_OBJECT

public:
    HttpServer();
    ~HttpServer();
    void init();
    void post(SPDOnceData data);

public slots:
    void resPost(QNetworkReply* reply);

signals:
    void setStatus(SPDOnceData::Detection_Result status);

public:
    QNetworkRequest *request;
    QNetworkAccessManager *nam;
    static const QString URL;
};