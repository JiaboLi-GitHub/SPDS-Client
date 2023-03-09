#pragma once

#include <QtWidgets/QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <qthread.h>

#include "SPDData.h"
#include "ui_Detection.h"
#include "VideoSurface.h"
#include "TcpData.h"
//#include "Module.h"

class Detection : public QWidget
{
    Q_OBJECT

public:
    Detection(QWidget *parent = nullptr);
    ~Detection();
    void startCamera();
    void stopCamera();
    bool isFreeze();

public slots:
    void on_start_clicked();
    void _presentframe(QVideoFrame& frame); 
    void setStatus(SPDOnceData::Detection_Result status);

signals:
    void forward(const QImage& captureImage);

private:
    Ui::DetectionClass ui;
    //ºÏ≤‚
    bool openSt;    // «∑Òº‡≤‚

    //…„œÒÕ∑
    QCamera* camera = NULL;
    VideoSurface* videoSurface = NULL;
    QImage img;
    //Module* module = NULL;
    //QThread* moduleThread = NULL;
};
