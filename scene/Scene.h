#ifndef __SCENE_H__
#define __SCENE_H__
#include <string>
#include "../utils/Geometry.h"
#include "../core/FrameBuffer.h"
#include "../core/Camera.h"
#include "Object.h"
#include "../GlobalConst.h"

class Scene {
public:
	~Scene();
	void init();
	void addModelObject(std::string path);
	void onRender();
	void moveCamera(MoveDir dir);
	void moveCameraFront(Vec3f offset);
private:
	std::vector<Object*> objects;
	Camera* camera;
};

#endif
