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

void test(const char path[])
{
    cv::Mat image = cv::imread(path);
    if (image.empty())
        fprintf(stderr, "Can not load image\n");

    // ת��ͨ��,
    cv::cvtColor(image, image, CV_BGR2RGB);
    cv::Mat img_float;
    image.convertTo(img_float, CV_32F, 1.0 / 255); //��һ��

    // resize, ����һ��������
    cv::resize(img_float, img_float, cv::Size(224, 224));
    //std::cout << img_float.at<cv::Vec3f>(256, 128)[1] << std::endl;

    // ת����tensor
    auto img_tensor = torch::from_blob(img_float.data, { 1, 3, 224, 224 }, torch::kFloat32);


    // ����input
    //auto img_var = torch::autograd::make_variable(img_tensor, false); //tensor->variable�ᱨ��
    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(img_tensor); //�������м����µ�Ԫ��, ��ֵ����

    //ǰ��
    at::Tensor output_image = ModuleLoader::module.forward(inputs).toTensor();  //�ó����
    std::cout << output_image<<std::endl;
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


    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\a1.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\a2.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\a3.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\b1.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\b2.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\b3.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\c1.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\c2.png");
    test("C:\\Users\\Gecko\\source\\repos\\JiaboLi-GitHub\\SPDS-Client\\c3.png");
}

/*************************************************
Description: �ر�����ͷ
*************************************************/
void Detection::stopCamera()
{
    moduleThread->terminate();
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