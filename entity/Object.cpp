#include "Object.h"

Matrix Object::getModelMat4() {
	Matrix mat4 = Matrix::identity();
	mat4[0][0] = scale_;
	mat4[1][1] = scale_;
	mat4[2][2] = scale_;
	mat4.set_col(3, embed<4>(pos_));
	return mat4;
}