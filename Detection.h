#pragma once

#include <QtWidgets/QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include "ui_Detection.h"
#include "VideoSurface.h"

class Detection : public QWidget
{
    Q_OBJECT

public:
    Detection(QWidget *parent = nullptr);
    ~Detection();
    void startCamera();
    void stopCamera();

public slots:
    void on_start_clicked();
    void _presentframe(QVideoFrame& frame);

private:
    Ui::DetectionClass ui;
    //ºÏ≤‚
    bool openSt;    // «∑Òº‡≤‚

    //…„œÒÕ∑
    QCamera* camera = NULL;
    VideoSurface *videoSurface=NULL;
    QImage img;
};
