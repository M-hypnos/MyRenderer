#pragma once
#include "../core/SCamera.h"
#include "ModelObject.h"
#include <vector>
#include "Light.h"
class Scene
{
public:
	Scene(float aspectRatio);
	~Scene();
	void addModelObject(const char* fileName);
	void addQuat(const char* fileName);
	void onRender();
	void resetCamera();
	void moveCamera(MoveDir dir);
	void updateFront(int x, int y, bool init);
	void updateFov(float y);
	void moveCameraFront(Vec3f offset);
	Vec3f getCameraEye() { return camera_->getEye(); }
	void leftLight();
	void rightLight();
private:
	void renderLightShader();
	SCamera* camera_;
	std::vector< Object*> objects_;
	std::vector<Light*> light_;
};

