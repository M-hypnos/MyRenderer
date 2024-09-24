#pragma once
#include "../GlobalConst.h"
#include "../utils/Math.h"
#include "ModelObject.h"
class Light
{
public:
	Light() {
		object_ = new ModelObject();
		object_->addModel("res/Crate/Crate1.obj");
		object_->setScale(scale);
		object_->setPos(Vec3f(0, 0, 0));
	}
	~Light() {
		delete object_;
	}
	void leftPress() {
		position = position + Vec3f(-1, 0, 0);
	}
	void rightPress() {
		position = position + Vec3f(1, 0, 0);
	}

	void onRender() {
		object_->setPos(position);
		object_->onRender();
	}

	Matrix getModelMat4() {
		Matrix mat4 = Matrix::identity();
		mat4[0][0] = scale;
		mat4[1][1] = scale;
		mat4[2][2] = scale;
		mat4.set_col(3, embed<4>(position));
		return mat4;
	}

	ModelObject* object_;
	Vec3f position;
	Vec3f intensity;
	Vec3f ambient;
	bool unOp;

	float scale = 0.3;
	int p;
};

