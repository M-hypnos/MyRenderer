#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../utils/Geometry.h"
#include "../GlobalConst.h"

class Camera {
public:
	Camera(Vec3f eye, Vec3f front, Vec3f up, float fov, float aspectRatio, float zNear, float zFar);
	Matrix getViewMat4();
	Matrix getProjectMat4();
	void setEyeOffset(MoveDir dir);
	void setFrontOffset(Vec3f offset);
private:
	Vec3f eye_;
	Vec3f front_;
	Vec3f up_;
	float fov_;
	float aspectRatio_;
	float zNear_;
	float zFar_;
};

#endif