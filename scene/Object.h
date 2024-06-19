#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <string>
#include "../core/Model.h"
#include "../utils/Geometry.h"

class Object {
public:
	Object();
	~Object();
	void addModel(std::string path);
	void onRender();
	Matrix getModelMat4();
	void setPos(Vec3f pos) { pos_ = pos; }
	void setScale(float scale) { scale_ = scale; }
private:
	Model* model_ = nullptr;
	Vec3f pos_;
	float scale_;
};

#endif
