#include "Detection.h"

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

    // 转换通道,
    cv::cvtColor(image, image, CV_BGR2RGB);
    cv::Mat img_float;
    image.convertTo(img_float, CV_32F, 1.0 / 255); //归一化

    // resize, 测试一个点数据
    cv::resize(img_float, img_float, cv::Size(224, 224));
    //std::cout << img_float.at<cv::Vec3f>(256, 128)[1] << std::endl;

    // 转换成tensor
    auto img_tensor = torch::from_blob(img_float.data, { 1, 3, 224, 224 }, torch::kFloat32);


    // 构造input
    //auto img_var = torch::autograd::make_variable(img_tensor, false); //tensor->variable会报错
    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(img_tensor); //向容器中加入新的元素, 右值引用

    //前向
    at::Tensor output_image = ModuleLoader::module.forward(inputs).toTensor();  //得出结果
    std::cout << output_image<<std::endl;
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
Description: 关闭摄像头
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
            ui.status->setText(u8"坐姿正常");
            break;
        case SPDData::Detection_Result::Head:
            ui.status->setText(u8"托头");
            break;
        case SPDData::Detection_Result::Front:
            ui.status->setText(u8"坐姿前倾");
            break;
        case SPDData::Detection_Result::Back:
            ui.status->setText(u8"坐姿后倾");
            break;
        case SPDData::Detection_Result::Left:
            ui.status->setText(u8"坐姿左倾");
            break;
        case SPDData::Detection_Result::Right:
            ui.status->setText(u8"坐姿右倾");
            break;
        }
    }
}