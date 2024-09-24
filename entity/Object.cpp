#include "Object.h"

Matrix Object::getModelMat4() {
	Matrix mat4 = Matrix::identity();
	mat4[0][0] = scale_;
	mat4[1][1] = scale_;
	mat4[2][2] = scale_;
	mat4.set_col(3, embed<4>(pos_));
	return mat4 * rotateMat_;
}

void Object::setRotate(Vec3f direct, float angle) {
	Matrix rotateMat = Matrix::identity();
	double radians = angle * 3.14159 / 180.f;
	float cosA = std::cos(radians);
	float sinA = std::sin(radians);
	rotateMat[0][0] = cosA + direct.x * direct.x * (1 - cosA);
	rotateMat[0][1] = direct.x * direct.y * (1 - cosA) - direct.z * sinA;
	rotateMat[0][2] = direct.y * sinA + direct.x * direct.z * (1 - cosA);
	rotateMat[1][0] = direct.z * sinA + direct.x * direct.y * (1 - cosA);
	rotateMat[1][1] = cosA + direct.y * direct.y * (1 - cosA);
	rotateMat[1][2] = -direct.x * sinA + direct.y * direct.z * (1 - cosA);
	rotateMat[2][0] = -direct.y * sinA + direct.x * direct.z * (1 - cosA);
	rotateMat[2][1] = direct.x * sinA + direct.y * direct.z * (1 - cosA);
	rotateMat[2][2] = cosA + direct.z * direct.z * (1 - cosA);
	rotateMat_ = rotateMat_ * rotateMat;
}