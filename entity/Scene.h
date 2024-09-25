#pragma once
#include "../core/Camera.h"
#include "ModelObject.h"
#include <vector>
#include "Light.h"
#include "../ui/Button.h"
class Scene
{
public:
	Scene(float aspectRatio, SDL_Renderer* renderer);
	~Scene();
	void addModelObject(const char* fileName, float scale = 1, int lightIdx = 0);
	void addQuat(const char* fileName);
	void onRender();
	void onRenderUI();
	void resetCamera();
	void moveCamera(MoveDir dir);
	void updateFront(int x, int y, bool init);
	void updateFov(float y);
	void moveCameraFront(Vec3f offset);
	Vec3f getCameraEye() { return camera_->getEye(); }
	void leftLight();
	void rightLight();
	void setObjectRotate(Vec3f direct, float angle);
	void checkClickBtn(int x, int y, bool isPress);
private:
	void renderTestShader();
	void renderLightShader();
	Camera* camera_;
	std::vector< Object*> objects_;
	SDL_Renderer* renderer_;
	std::vector< Button*> button_;

	std::vector<std::vector<Light*>> lightPool_;
	bool openLight_ = true;
	int lightIdx_;
};

