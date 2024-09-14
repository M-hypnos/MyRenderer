#pragma once
#include "../core/SCamera.h"
#include "Object.h"
#include <vector>
class Scene
{
public:
	Scene(float aspectRatio);
	~Scene();
	void addModelObject(const char* fileName);
	void onRender();
	void resetCamera();
	void setLineMode();
	void setTriangleMode();
	void moveCamera(MoveDir dir);
	void updateFront(int x, int y, bool init);
	void updateFov(float y);
	void moveCameraFront(Vec3f offset);
private:
	SCamera* camera_;
	std::vector< Object*> objects_;
};

