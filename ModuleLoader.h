#pragma once

#include <QObject>

#undef slots
//#include "torch/torch.h"
#include "torch/script.h"
#define slots Q_SLOTS

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/types_c.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/videoio/videoio.hpp>

class ModuleLoader  : public QObject
{
	Q_OBJECT

public:
	ModuleLoader(QObject *parent);
	~ModuleLoader();

public:
	static torch::jit::script::Module module;
};
