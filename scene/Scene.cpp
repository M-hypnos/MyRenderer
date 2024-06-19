#include "Scene.h"
using namespace std;
#include "../shader/TestShader.h"
#include "../core/Pipeline.h"
Scene::~Scene() {
	if(camera != nullptr) delete camera;
	for (auto object : objects) {
		delete object;
	}
}

void Scene::init() {
	camera = new Camera(Vec3f(0, 0, 30), Vec3f(0, 0, -15), Vec3f(0, 1, 0), 45.f, 1.f, 0.1f, 50.f);
	//camera = new Camera(Vec3f(0, 0, 10), Vec3f(0, 0, 0), Vec3f(0, 1, 0), 45.f, 1.f, 1.f, 50.f);
	addModelObject("res/nanosuit/nanosuit.obj");
}

void Scene::addModelObject(string path) {
	Object *object = new Object();
	object->addModel(path);
	//object->setScale(0.05);
	object->setScale(1.5);
	object->setPos(Vec3f(0, -10, 0));
	objects.push_back(object);
}

void Scene::moveCamera(MoveDir dir) {
	if (camera == nullptr) return;
	camera->setEyeOffset(dir);
}

void Scene::moveCameraFront(Vec3f offset) {
	if (camera == nullptr) return;
	camera->setFrontOffset(offset);
}


void Scene::onRender() {
	TestShader* shader = new TestShader();
	PipeLine::getInstance().bindShader(shader);
	PipeLine::getInstance().setRendererMode(RendererMode::Point);
	PipeLine::getInstance().setRendererMode(RendererMode::Line);
	PipeLine::getInstance().setRendererMode(RendererMode::Triangle);
	shader->viewMat4 = camera->getViewMat4();
	shader->projectMat4 = camera->getProjectMat4();

	for (auto object : objects) {
		shader->modelMat4 = object->getModelMat4();
		object->onRender();
	}

	delete shader;
}