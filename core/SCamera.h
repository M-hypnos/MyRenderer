#pragma once
#include "../utils/Math.h"
#include "../GlobalConst.h"
class SCamera
{
public:
	SCamera(Vec3f eye, Vec3f front, Vec3f up, float fov, float aspectRatio, float zNear, float zFar);
	void setParam(Vec3f eye, Vec3f front, Vec3f up, float fov);
	Matrix getViewMat4();
	Matrix getProjectMat4();
	void setEyeOffset(MoveDir dir);
	void updateFront(int x, int y, bool init);
	void updateFov(float y);
	void setFrontOffset(Vec3f offset);
private:
	Vec3f eye_;
	Vec3f front_;
	Vec3f up_;
	float fov_;
	float aspectRatio_;
	float zNear_;
	float zFar_;
	int lastX_;
	int lastY_;

	float pitch_ = 0.f;
	float yaw_ = -90.f;
};

