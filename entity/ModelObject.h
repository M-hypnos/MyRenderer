#pragma once
#include "Object.h"
#include "../core/SObj_Loader.h"
#include "../core/SPipeline.h"
class ModelObject :
    public Object
{
public:
	~ModelObject() {
		if (obj_loader != nullptr)
			delete obj_loader;
	}

	void addModel(const char* filename) {
		obj_loader = new SObj_Loader();
		obj_loader->loadFile(filename);
	}

	void onRender() override {
		SPipeline::getInstance().drawObj(obj_loader);
	}
private:
    SObj_Loader* obj_loader;
};

