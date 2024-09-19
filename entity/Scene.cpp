#include "Scene.h"
#include "../shader/TestShader.h"
#include "../shader/LightShader.h"
#include "../core/SPipeline.h"
#include "../entity/QuatObject.h"

Vec3f cameraEye = Vec3f(0, 0, 15);
Vec3f cameraFront = Vec3f(0, 0, -1);
Vec3f cameraUp = Vec3f(0, 1, 0);
float cameraFov = 45.f;

Scene::Scene(float aspectRatio) {
	camera_ = new SCamera(cameraEye, cameraFront, cameraUp, cameraFov, aspectRatio, 0.1f, 50.f);

	/*Light* l = new Light();
	l->direct_ = Vec3f(-1,-1,-1);
	l->ambient_ = Vec3f(0.05f, 0.05f, 0.05f);
	l->diffuse_ = Vec3f(0.8f, 0.8f, 0.8f);
	l->specular_ = Vec3f(1.0f, 1.0f, 1.0f);
	light_.emplace_back(l);*/

	Light* l1 = new Light(LightType::Point);
	l1->position_ = Vec3f(3, 5, 2);
	l1->ambient_ = Vec3f(0.05f, 0.05f, 0.05f);
	l1->diffuse_ = Vec3f(0.8f, 0.8f, 0.8f);
	l1->specular_ = Vec3f(1.0f, 1.0f, 1.0f);
	l1->constant_ = 1.f;
	l1->linear_ = 0.09f;
	l1->quadratic_ = 0.032f;
	light_.emplace_back(l1);
	/*Light* l2 = new Light(LightType::Point);
	l2->position_ = Vec3f(-3, -5, 3);
	l2->ambient_ = Vec3f(0.05f, 0.05f, 0.05f);
	l2->diffuse_ = Vec3f(0.8f, 0.8f, 0.8f);
	l2->specular_ = Vec3f(1.0f, 1.0f, 1.0f);
	l2->constant_ = 1.f;
	l2->linear_ = 0.09f;
	l2->quadratic_ = 0.032f;
	light_.emplace_back(l2);*/
}

Scene::~Scene() {
	if (camera_ != nullptr)
		delete camera_;
	for (auto light : light_) {
		delete light;
	}
	for (auto object : objects_) {
		delete object;
	}
}

void Scene::addModelObject(const char* filename) {
	ModelObject* object = new ModelObject();
	object->addModel(filename);
	//object->setScale(0.01);
	object->setScale(1);
	object->setPos(Vec3f(0, 0, 0));
	objects_.push_back(object);
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
	renderLightShader();
}

void Scene::renderLightShader() {
	LightShader* shader = new LightShader();
	SPipeline::getInstance().bindShader(shader);
	shader->viewMat4 = camera_->getViewMat4();
	shader->projectMat4 = camera_->getProjectMat4();
	shader->light = light_;
	shader->viewPos = camera_->getEye();
	for (auto obj : objects_) {
		shader->modelMat4 = obj->getModelMat4();
		obj->onRender();
	}
	delete shader;
}

void Scene::resetCamera() {
	if (camera_ == nullptr) return;
	camera_->setParam(cameraEye, cameraFront, cameraUp, cameraFov);
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
	light_[0]->leftPress();
}

void Scene::rightLight() {
	light_[0]->rightPress();
}