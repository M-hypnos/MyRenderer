#pragma once
//#include "../core/SModel.h"
#include "../core/SObj_Loader.h"
class Object
{
public:
	~Object();
	void addModel(const char* filename);
	void onRender();
	Matrix getModelMat4();
	void setPos(Vec3f pos) { pos_ = pos; }
	void setScale(float scale) { scale_ = scale; }
private:
	Vec3f pos_;
	float scale_;
	//SModel* model_;
	SObj_Loader* obj_loader;
};

