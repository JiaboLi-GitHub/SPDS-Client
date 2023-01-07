#include "Detection.h"


Detection::Detection(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //按钮不显示边框
    ui.start->setFlat(true);
    openSt = false;

    QImage image;
    image.load(":/SPDS_Client/resources/icon/detection_background_error.png");
    ui.frame->setScaledContents(true);
    ui.frame->setPixmap(QPixmap::fromImage(image));
}

Detection::~Detection()
{
    delete camera;
    delete videoSurface;
}

void Detection::on_start_clicked()
{
    openSt  = !openSt;
    if (openSt)
    {
        ui.start->setIcon(QIcon(":/SPDS_Client/resources/icon/stop.png"));
        startCamera();
    }
    else
    {
        ui.start->setIcon(QIcon(":/SPDS_Client/resources/icon/start.png"));
        stopCamera();
    }
}

/*-------------------------------摄像头------------------------------------*/
//打开摄像头
void Detection::startCamera()
{
    QCameraInfo cameraInfo = QCameraInfo::availableCameras().at(0);
    //创建摄像头对象
    camera = new QCamera(cameraInfo);
    
    videoSurface = new VideoSurface(ui.start);
    camera->setViewfinder(videoSurface);
    connect(videoSurface, &VideoSurface::presentframe_, this, &Detection::_presentframe);
    camera->start();
}

//关闭摄像头
void Detection::stopCamera()
{
    ui.cameraStart->clear();
    camera->stop();
    delete videoSurface;
    delete camera;
    camera = NULL;
    videoSurface = NULL;
}

//每一帧
void Detection::_presentframe(QVideoFrame& frame)
{
    frame.map(QAbstractVideoBuffer::ReadOnly);
    img= QImage(frame.bits(), frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())).copy();
    frame.unmap();
    
    img.save("a.jpg");


    QMatrix matrix;
    matrix.rotate(180);
    img = img.transformed(matrix);
    ui.cameraStart->setPixmap(QPixmap::fromImage(img));
    ui.cameraStart->setScaledContents(true);
}

