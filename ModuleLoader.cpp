#include "ModuleLoader.h"

torch::jit::script::Module ModuleLoader::module = torch::jit::load("./resources/module/resnet50_model.pt");

ModuleLoader::ModuleLoader(QObject *parent)
	: QObject(parent)
{}

ModuleLoader::~ModuleLoader()
{}
