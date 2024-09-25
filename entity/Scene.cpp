#include "Scene.h"
#include "../shader/TexShader.h"
#include "../shader/LightShader.h"
#include "../shader/WhiteShader.h"
#include "../core/Pipeline.h"
#include "../entity/QuatObject.h"

Vec3f cameraEye = Vec3f(0, 0, 15);
Vec3f cameraFront = Vec3f(0, 0, -1);
Vec3f cameraUp = Vec3f(0, 1, 0);
float cameraFov = 45.f;

Scene::Scene(float aspectRatio, SDL_Renderer* renderer) {
	camera_ = new Camera(cameraEye, cameraFront, cameraUp, cameraFov, aspectRatio, 0.1f, 50.f);
	renderer_ = renderer;

	int w = 100, h = 50;
	Button* btn1 = new Button(0, 0, w, h, []()->void {
		Pipeline::getInstance().switchRendererMode();
	}, "mode", renderer_);
	button_.emplace_back(btn1);

	Button* btn2 = new Button(0, h, w, h, []()->void {
		Pipeline::getInstance().switchBackCulling();
	}, "backCulling", renderer_);
	button_.emplace_back(btn2);

	Button* btn3 = new Button(0, 2 * h, w, h, []()->void {
		Pipeline::getInstance().switchClipTriangle();
	}, "clipTriangle", renderer_);
	button_.emplace_back(btn3);

	Button* btn4 = new Button(0, 3 * h, w, h, [&]()->void {
		openLight_ = !openLight_;
	}, "lightShader", renderer_);
	button_.emplace_back(btn4);

	{
		Light* l = new Light();
		l->position = Vec3f(5.f, 5.f, 5.f);
		l->ambient = Vec3f(0.3f, 0.3f, 0.3f);
		l->intensity = Vec3f(100.f, 100.f, 100.f);
		l->p = 150;
		std::vector<Light*> lp;
		lp.emplace_back(l);
		lightPool_.emplace_back(lp);
	}

	{
		Light* l = new Light();
		l->position = Vec3f(7.f, 7.f, 5.f);
		l->ambient = Vec3f(0.2f, 0.2f, 0.2f);
		l->intensity = Vec3f(100.f, 100.f, 100.f);
		l->p = 150;
		std::vector<Light*> lp;
		lp.emplace_back(l);
		lightPool_.emplace_back(lp);
	}

	{
		std::vector<Light*> lp;
		Light* l1 = new Light();
		l1->position = Vec3f(20.f, 10.f, 10.f);
		l1->ambient = Vec3f(0.1f, 0.1f, 0.1f);
		l1->intensity = Vec3f(500.f, 500.f, 500.f);
		l1->p = 150;
		lp.emplace_back(l1);

		Light* l2 = new Light();
		l2->position = Vec3f(-5.f, 25.f, 2.f);
		l2->ambient = Vec3f(0.1f, 0.1f, 0.1f);
		l2->intensity = Vec3f(500.f, 500.f, 500.f);
		l2->p = 150;
		lp.emplace_back(l2);
		l2->unOp = true;
		lightPool_.emplace_back(lp);
	}
}

Scene::~Scene() {
	if (camera_ != nullptr)
		delete camera_;
	for (auto lights : lightPool_) {
		for(auto light: lights)
			delete light;
	}
	for (auto object : objects_) {
		delete object;
	}
	for (auto btn : button_) {
		delete btn;
	}
}

void Scene::addModelObject(const char* filename, float scale, int lightIdx) {
	ModelObject* object = new ModelObject();
	object->addModel(filename);
	object->setScale(scale);
	object->setPos(Vec3f(0, 0, 0));
	objects_.push_back(object);
	lightIdx_ = lightIdx;
}

void Scene::addQuat(const char* filename) {
	std::vector<Vertex> verts;
	verts.emplace_back(Vec3f(-1, 0, 1), Vec2f(0, 0), Vec3f(0,1,0));
	verts.emplace_back(Vec3f(1, 0, -1), Vec2f(1, 1), Vec3f(0, 1, 0));
	verts.emplace_back(Vec3f(-1, 0, -1), Vec2f(0, 1), Vec3f(0, 1, 0));
	verts.emplace_back(Vec3f(-1, 0, 1), Vec2f(0, 0), Vec3f(0, 1, 0));
	verts.emplace_back(Vec3f(1, 0, 1), Vec2f(1, 0), Vec3f(0, 1, 0));
	verts.emplace_back(Vec3f(1, 0, -1), Vec2f(1, 1), Vec3f(0, 1, 0));
	QuatObject* object = new QuatObject();
	object->addQuat(verts, filename);
	object->setPos(Vec3f(0, -5, 0));
	object->setScale(3);
	objects_.push_back(object);
}


void Scene::onRender() {
	if (openLight_) {
		renderLightShader();
	}
	else {
		renderTestShader();
	}
}

void Scene::onRenderUI() {
	for (auto btn : button_) {
		btn->onRender(renderer_);
	}
}

void Scene::renderTestShader() {
	TexShader* shader = new TexShader();
	Pipeline::getInstance().bindShader(shader);
	shader->viewMat4 = camera_->getViewMat4();
	shader->projectMat4 = camera_->getProjectMat4();
	for (auto obj : objects_) {
		shader->modelMat4 = obj->getModelMat4();
		shader->MVT = (shader->viewMat4 * shader->modelMat4).invert_transpose();
		obj->onRender();
	}
	delete shader;
}

void Scene::renderLightShader() {
	LightShader* shader = new LightShader();
	Pipeline::getInstance().bindShader(shader);
	shader->viewMat4 = camera_->getViewMat4();
	shader->projectMat4 = camera_->getProjectMat4();
	shader->light = lightPool_[lightIdx_];
	shader->viewPos = camera_->getEye();
	for (auto obj : objects_) {
		shader->modelMat4 = obj->getModelMat4();
		shader->MVT = (shader->viewMat4 * shader->modelMat4).invert_transpose();
		obj->onRender();
	}
	delete shader;

	WhiteShader* lshader = new WhiteShader();
	Pipeline::getInstance().bindShader(lshader);
	lshader->viewMat4 = camera_->getViewMat4();
	lshader->projectMat4 = camera_->getProjectMat4();
	for (auto l : lightPool_[lightIdx_]) {
		lshader->modelMat4 = l->getModelMat4();
		shader->MVT = (shader->viewMat4 * shader->modelMat4).invert_transpose();
		l->onRender();
	}
	delete lshader;
}

void Scene::resetCamera() {
	if (camera_ == nullptr) return;
	camera_->setParam(cameraEye, cameraFront, cameraUp, cameraFov);
	if (objects_.size() > 0) {
		objects_[0]->resetRotate();
	}
}

void Scene::moveCamera(MoveDir dir) {
	if (camera_ == nullptr) return;
	camera_->setEyeOffset(dir);
}

void Scene::updateFront(int x, int y, bool init) {
	if (camera_ == nullptr) return;
	camera_->updateFront(x, y, init);
}

void Scene::updateFov(float y) {
	if (camera_ == nullptr) return;
	camera_->updateFov(y);
}

void Scene::moveCameraFront(Vec3f offset) {
	if (camera_ == nullptr) return;
	camera_->setFrontOffset(offset);
}

void Scene::leftLight() {
	for (auto light : lightPool_[lightIdx_]) {
		if (light->unOp) continue;
		light->leftPress();
	}
}

void Scene::rightLight() {
	for (auto light : lightPool_[lightIdx_]) {
		if (light->unOp) continue;
		light->rightPress();
	}
}

void Scene::setObjectRotate(Vec3f direct, float angle) {
	if (objects_.size() > 0) {
		objects_[0]->setRotate(direct, angle);
	}
}

void Scene::checkClickBtn(int x, int y, bool isPress) {
	for (auto btn : button_) {
		if (isPress)
			btn->onPressDown(x, y);
		else
			btn->onPressUp();
	}
}