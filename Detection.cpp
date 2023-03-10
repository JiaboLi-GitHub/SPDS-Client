#include "Detection.h"
#include "SPDData.h"
#include "SPDS_Client.h"

#include <qmessagebox.h>
#include <QDateTime>
#include <QMetaType>

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

    httpserver = new HttpServer();
    connect(httpserver, &HttpServer::setStatus, this, &Detection::setStatus);
    lastTime = QDateTime::currentDateTime();

    //module = new Module(this);
    //moduleThread = new QThread(this);
    //module->moveToThread(moduleThread);

    //connect(this, SIGNAL(forward(const QImage&)), module, SLOT(forward(const QImage&)), Qt::AutoConnection);
    //qRegisterMetaType<SPDData::Detection_Result>("SPDData::Detection_Result");
    //connect(this, &Detection::forward, module, &Module::forward, Qt::QueuedConnection);
    //connect(module, &Module::setStatus, this, &Detection::setStatus, Qt::QueuedConnection);

    //moduleThread->start();
}

Detection::~Detection()
{
    //moduleThread->terminate();

    delete camera;
    delete videoSurface;
}


void Detection::on_start_clicked()
{
    if (!((SPDS_Client*)this->parent())->isLogined())
    {
        QMessageBox::warning(NULL, u8"用户未登录", u8"请先登录再使用该功能！", QMessageBox::Ok);
        return;
    }

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
/*************************************************
Description: 打开摄像头
*************************************************/
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

/*************************************************
Description: 关闭摄像头
*************************************************/
void Detection::stopCamera()
{
    ui.cameraStart->clear();
    camera->stop();
    delete videoSurface;
    delete camera;
    camera = NULL;
    videoSurface = NULL;
}

bool Detection::isFreeze()
{
    return openSt;
}

/*************************************************
Description: 将视频帧旋转后显示在界面上
      Input: frame=视频帧
*************************************************/
void Detection::_presentframe(QVideoFrame& frame)
{
    frame.map(QAbstractVideoBuffer::ReadOnly);
    img= QImage(frame.bits(), frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())).copy();
    frame.unmap();
    
    //img.save("a.jpg");

    QMatrix matrix;
    matrix.rotate(180);
    img = img.transformed(matrix);

    QDateTime currentTime = QDateTime::currentDateTime();
    if (currentTime.msecsTo(lastTime) >= timeGap)
    {
        httpserver->post(SPDOnceData(currentTime.date(), img));
        lastTime = currentTime;
    }
    //emit forward(img);

    ui.cameraStart->setPixmap(QPixmap::fromImage(img));
    ui.cameraStart->setScaledContents(true);
}

void Detection::setStatus(SPDOnceData::Detection_Result status)
{
    switch (status)
    {
    case SPDOnceData::Detection_Result::Normal:
        ui.status->setText(u8"坐姿正常");
        break;
    case SPDOnceData::Detection_Result::Head:
        ui.status->setText(u8"托头");
        break;
    case SPDOnceData::Detection_Result::Front:
        ui.status->setText(u8"坐姿前倾");
        break;
    case SPDOnceData::Detection_Result::Back:
        ui.status->setText(u8"坐姿后倾");
        break;
    case SPDOnceData::Detection_Result::Left:
        ui.status->setText(u8"坐姿左倾");
        break;
    case SPDOnceData::Detection_Result::Right:
        ui.status->setText(u8"坐姿右倾");
        break;
    }
}