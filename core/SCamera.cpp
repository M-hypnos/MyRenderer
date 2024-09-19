#include "SCamera.h"
#include <cmath>

SCamera::SCamera(Vec3f eye, Vec3f front, Vec3f up, float fov, float aspectRatio, float zNear, float zFar)
	:eye_(eye)
	, front_((front).normalize())
	, up_((up).normalize())
	, fov_(fov)
	, aspectRatio_(aspectRatio)
	, zNear_(zNear)
	, zFar_(zFar) {

}

void SCamera::setParam(Vec3f eye, Vec3f front, Vec3f up, float fov) {
	eye_ = eye;
	front_ = front;
	up_ = up;
	fov_ = fov;
	pitch_ = 0.f;
	yaw_ = -90.f;
}

Matrix SCamera::getViewMat4() {
	Matrix move = Matrix::identity();
	Vec3f z = (front_ * -1).normalize();
	Vec3f x = (cross(up_, z)).normalize();
	Vec3f y = (cross(z, x)).normalize();
	move[0][3] = -eye_.x;
	move[1][3] = -eye_.y;
	move[2][3] = -eye_.z;
	Matrix rotate = Matrix::identity();
	for (int i = 0; i < 3; i++) {
		rotate[0][i] = x[i];
		rotate[1][i] = y[i];
		rotate[2][i] = z[i];
	}
	return rotate * move;
}

Matrix SCamera::getProjectMat4() {
	Matrix matO = Matrix::identity();
	float t = zNear_ * std::tan(fov_ / 2 * 3.141 / 180);
	float r = t * aspectRatio_;
	float n = -zNear_;
	float f = -zFar_;
	matO[0][0] = 1 / r;
	matO[1][1] = 1 / t;
	matO[2][2] = 2 / (n - f);
	matO[2][3] = -(n + f) / (n - f);

	Matrix matP = Matrix::identity();
	matP[0][0] = zNear_;
	matP[1][1] = zNear_;
	matP[2][2] = zNear_ + zFar_;
	matP[2][3] = zNear_ * zFar_;
	matP[3][2] = -1;
	matP[3][3] = 0;

	Matrix mp = matO * matP;
	return mp;
}

void SCamera::setEyeOffset(MoveDir dir) {
	Vec3f z = (front_ * -1).normalize();
	Vec3f x = (cross(up_, z)).normalize();
	Vec3f y = (cross(z, x)).normalize();

	switch (dir)
	{
	case MoveDir::Bottom:
		eye_ = eye_ + Vec3f(0,-2,0);
		break;
	case MoveDir::Top:
		eye_ = eye_ + Vec3f(0,2,0);
		break;
	case MoveDir::Left:
		eye_ = eye_ + x * (-2);
		break;
	case MoveDir::Right:
		eye_ = eye_ + x * (2);
		break;
	case MoveDir::Forward:
		eye_ = eye_ + z * (-2);
		break;
	case MoveDir::Back:
		eye_ = eye_ + z * (2);
		break;
	default:
		break;
	}
}

void SCamera::updateFront(int x, int y, bool init) {
	if (init) {
		lastX_ = x;
		lastY_ = y;
		return;
	}
	int xOffset = x - lastX_;
	int yOffset = lastY_ - y;

	lastX_ = x;
	lastY_ = y;

	float sensitivity = 0.05f;

	yaw_ += (xOffset * sensitivity);
	pitch_ += (yOffset * sensitivity);
	if (pitch_ > 89.f)
		pitch_ = 89.f;
	if (pitch_ < -89.f)
		pitch_ = -89.f;

	float pr = pitch_ * 3.14159 / 180;
	float yr = yaw_ * 3.14159 / 180;
	front_.x = cos(pr) * cos(yr);
	front_.y = sin(pr);
	front_.z = cos(pr) * sin(yr);
	front_.normalize();
}

void SCamera::updateFov(float y) {
	if (fov_ >= 1.0f && fov_ <= 45.0f)
		fov_ -= y;
	if (fov_ <= 1.0f)
		fov_ = 1.0f;
	if (fov_ >= 45.0f)
		fov_ = 45.0f;
}

void SCamera::setFrontOffset(Vec3f offset) {
	front_ = front_ + offset;
}