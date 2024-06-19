#include "Object.h"
using namespace std;

Object::Object()
: pos_(Vec3f())
, scale_(1.f)
{}

Object::~Object() {
	if (model_ != nullptr)
		delete model_;
}

void Object::addModel(string path) {
	model_ = new Model(path);
}

void Object::onRender() {
	if(model_ != nullptr)
		model_->onRender();
}

Matrix Object::getModelMat4() {
	Matrix mat4 = Matrix::identity();
	mat4[0][0] = scale_;
	mat4[1][1] = scale_;
	mat4[2][2] = scale_;
	mat4.set_col(3, embed<4>(pos_));
	return mat4;
}