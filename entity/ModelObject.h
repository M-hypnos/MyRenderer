#pragma once
#include "Object.h"
#include "../core/Obj_Loader.h"
#include "../core/Pipeline.h"
class ModelObject :
    public Object
{
public:
	~ModelObject() {
		if (obj_loader != nullptr)
			delete obj_loader;
	}

	void addModel(const char* filename) {
		obj_loader = new Obj_Loader();
		obj_loader->loadFile(filename);
	}

	void onRender() override {
		Pipeline::getInstance().drawObj(obj_loader);
	}
private:
    Obj_Loader* obj_loader;
};

