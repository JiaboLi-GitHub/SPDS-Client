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
    //��ť����ʾ�߿�
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
        QMessageBox::warning(NULL, u8"�û�δ��¼", u8"���ȵ�¼��ʹ�øù��ܣ�", QMessageBox::Ok);
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

/*-------------------------------����ͷ------------------------------------*/
/*************************************************
Description: ������ͷ
*************************************************/
void Detection::startCamera()
{
    QCameraInfo cameraInfo = QCameraInfo::availableCameras().at(0);
    //��������ͷ����
    camera = new QCamera(cameraInfo);
    
    videoSurface = new VideoSurface(ui.start);
    camera->setViewfinder(videoSurface);
    connect(videoSurface, &VideoSurface::presentframe_, this, &Detection::_presentframe);
    camera->start();
}

/*************************************************
Description: �ر�����ͷ
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
Description: ����Ƶ֡��ת����ʾ�ڽ�����
      Input: frame=��Ƶ֡
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
        ui.status->setText(u8"��������");
        break;
    case SPDOnceData::Detection_Result::Head:
        ui.status->setText(u8"��ͷ");
        break;
    case SPDOnceData::Detection_Result::Front:
        ui.status->setText(u8"����ǰ��");
        break;
    case SPDOnceData::Detection_Result::Back:
        ui.status->setText(u8"���˺���");
        break;
    case SPDOnceData::Detection_Result::Left:
        ui.status->setText(u8"��������");
        break;
    case SPDOnceData::Detection_Result::Right:
        ui.status->setText(u8"��������");
        break;
    }
}