#include "Object.h"
#include "../core/SPipeline.h"

Object::~Object() {
	if (obj_loader != nullptr)
		delete obj_loader;
}

void Object::addModel(const char* filename) {
	obj_loader = new SObj_Loader();
	obj_loader->loadFile(filename);
}

void Object::onRender() {
	SPipeline::getInstance().drawObj(obj_loader);
}

Matrix Object::getModelMat4() {
	Matrix mat4 = Matrix::identity();
	mat4[0][0] = scale_;
	mat4[1][1] = scale_;
	mat4[2][2] = scale_;
	mat4.set_col(3, embed<4>(pos_));
	return mat4;
}