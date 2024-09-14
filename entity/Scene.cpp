#include "Scene.h"
#include "../shader/TestShader.h"
#include "../core/SPipeline.h"

Vec3f cameraEye = Vec3f(0, 0, 30);
Vec3f cameraFront = Vec3f(0, 0, -1);
Vec3f cameraUp = Vec3f(0, 1, 0);
float cameraFov = 45.f;

Scene::Scene(float aspectRatio) {
	camera_ = new SCamera(cameraEye, cameraFront, cameraUp, cameraFov, aspectRatio, 0.1f, 50.f);
}

Scene::~Scene() {
	if (camera_ != nullptr)
		delete camera_;
	for (auto object : objects_) {
		delete object;
	}
}

void Scene::addModelObject(const char* filename) {
	Object* object = new Object();
	object->addModel(filename);
	//object->setScale(0.01);
	object->setScale(3);
	object->setPos(Vec3f(0, 0, 0));
	objects_.push_back(object);
}


void Scene::onRender() {
	TestShader* shader = new TestShader();
	SPipeline::getInstance().bindShader(shader);
	shader->viewMat4 = camera_->getViewMat4();
	shader->projectMat4 = camera_->getProjectMat4();
	for (auto obj : objects_) {
		shader->modelMat4 = obj->getModelMat4();
		obj->onRender();
	}

	delete shader;
}

void Scene::setLineMode() {
	SPipeline::getInstance().setRendererMode(RendererMode::Line);
}

void Scene::setTriangleMode() {
	SPipeline::getInstance().setRendererMode(RendererMode::Triangle);
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