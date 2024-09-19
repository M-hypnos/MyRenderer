#pragma once
#include "../utils/Math.h"
class Object
{
public:
	virtual ~Object() = default;
	virtual void onRender() = 0;
	Matrix getModelMat4();
	void setPos(Vec3f pos) { pos_ = pos; }
	void setScale(float scale) { scale_ = scale; }
private:
	Vec3f pos_;
	float scale_;	
};

