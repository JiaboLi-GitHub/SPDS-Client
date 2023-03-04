#include "Module.h"

Module::Module(QObject* parent)
{
	prevStatus = SPDData::Detection_Result::Head;
}

Module::~Module()
{}

std::vector<int64_t> Module::sizes = { 1, 3, 224, 224 };

cv::Mat Module::qim2mat(const QImage& qimg)
{
	cv::Mat mat = cv::Mat(qimg.height(), qimg.width(),
		CV_8UC3, (void*)qimg.constBits(), qimg.bytesPerLine());
	return mat;
}

void Module::forward(const QImage& captureImage)
{
	// 读取图片
	cv::Mat image = qim2mat(captureImage);
	if (image.empty())
		fprintf(stderr, "Can not load image\n");

	// 转换通道,
	cv::cvtColor(image, image, CV_BGR2RGB);
	cv::Mat img_float;
	image.convertTo(img_float, CV_32F, 1.0 / 255); //归一化

	// resize, 测试一个点数据
	cv::resize(img_float, img_float, cv::Size(224, 224));

	//将opencv的图像数据转为Tensor张量数据
	at::Tensor tensor_image = torch::from_blob(img_float.data, at::IntList(sizes), at::TensorOptions(at::ScalarType::Byte));

	//转为浮点型张量数据
	tensor_image = tensor_image.toType(at::kFloat);

	auto result = ModuleLoader::module.forward({ tensor_image }).toTensor().argmax().item<int>();

	SPDData::Detection_Result status = SPDData::Detection_Result(result);

	if (status != prevStatus)
	{
		prevStatus = status;
		emit setStatus(prevStatus);
	}

	return;
}