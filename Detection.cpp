#include "Detection.h"

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

    module = new Module(this);
    moduleThread = new QThread(this);
    module->moveToThread(moduleThread);

    //connect(this, SIGNAL(forward(const QImage&)), module, SLOT(forward(const QImage&)), Qt::AutoConnection);
    qRegisterMetaType<SPDData::Detection_Result>("SPDData::Detection_Result");
    connect(this, &Detection::forward, module, &Module::forward, Qt::QueuedConnection);
    connect(module, &Module::setStatus, this, &Detection::setStatus, Qt::QueuedConnection);

    moduleThread->start();
}

Detection::~Detection()
{
    moduleThread->terminate();

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

    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    emit forward(img);

    ui.cameraStart->setPixmap(QPixmap::fromImage(img));
    ui.cameraStart->setScaledContents(true);
}

void Detection::setStatus(SPDData::Detection_Result status)
{
    if (status)
    {
        switch (status)
        {
        case SPDData::Detection_Result::Normal:
            ui.status->setText(u8"��������");
            break;
        case SPDData::Detection_Result::Head:
            ui.status->setText(u8"��ͷ");
            break;
        case SPDData::Detection_Result::Front:
            ui.status->setText(u8"����ǰ��");
            break;
        case SPDData::Detection_Result::Back:
            ui.status->setText(u8"���˺���");
            break;
        case SPDData::Detection_Result::Left:
            ui.status->setText(u8"��������");
            break;
        case SPDData::Detection_Result::Right:
            ui.status->setText(u8"��������");
            break;
        }
    }
}