#include "SceneBase.h"

void SceneBase::addLightObject(Object* lightObject)
{
	lightManager->addLight(lightObject);
}

void SceneBase::removeLightObject(Object* lightObject)
{
	lightManager->removeLight(lightObject);
}

std::vector<Object*>* SceneBase::getLights()
{
	return lightManager->getLights();
}

void SceneBase::fetchLights()
{
	if (root == nullptr) return;

	// setup
	std::vector<Object*> objectStack = { root };

	Object* curObject;
	while (!objectStack.empty()) {
		// stack operation
		curObject = objectStack.back();
		objectStack.pop_back();
		for (auto it : *(curObject->getChildren())) {
			objectStack.push_back(it);
		}

		// check for objects with light property
		LightProperty* curObjectLightProperty = (LightProperty*)curObject->findProperty(PropertyType::LIGHT);
		if (curObjectLightProperty != nullptr)
		{
			addLightObject(curObject);
		}
	}
}

SceneBase::SceneBase(WindowInstance* _window, Object* _root, Camera* _camera)
	:window(_window)
	, root(_root)
	, camera(_camera)
	, lightManager(new LightManager())
{
}

SceneBase::SceneBase(WindowInstance* _window)
	:window(_window)
	, root(nullptr)
	, camera(nullptr)
	, lightManager(new LightManager())
{
}

bool SceneBase::init()
{
	return true;
}

bool SceneBase::update() {
	if (root == nullptr) return false;

	// setup
	std::vector<Object*> objectStack = { root };

	Object* curObject;
	while (!objectStack.empty()) {
		// stack operation
		curObject = objectStack.back();
		objectStack.pop_back();
		for (auto it : *(curObject->getChildren())) {
			objectStack.push_back(it);
		}

		curObject->update();
	}

	objectStack.push_back(root);
	while (!objectStack.empty()) {
		// stack operation
		curObject = objectStack.back();
		objectStack.pop_back();
		for (auto it : *(curObject->getChildren())) {
			objectStack.push_back(it);
		}

		curObject->applyOrigin();
	}

	return true;
}

bool SceneBase::render()
{
	if (camera == nullptr || root == nullptr) return false;

	// setup
	camera->prerender();
	std::vector<Object*> objectStack = { root };

	Object* curObject;
	while (!objectStack.empty()) {
		//stack operation
		curObject = objectStack.back();
		objectStack.pop_back();
		for (auto it : *(curObject->getChildren())) {
			objectStack.push_back(it);
		}

		// check for objects with mesh property
		MeshProperty* curObjectMeshProperty = (MeshProperty*)curObject->findProperty(PropertyType::MESH);
		if (curObjectMeshProperty != nullptr)
		{
			Mesh* curObjectMesh = curObjectMeshProperty->mesh;
			camera->renderMesh(curObject, curObjectMesh, getLights());
		}

		// check for objects with textured mesh property
		TexturedMeshProperty* curObjectTexturedMeshProperty = (TexturedMeshProperty*)curObject->findProperty(PropertyType::TEXTURED_MESH);
		if (curObjectTexturedMeshProperty != nullptr)
		{
			TexturedMesh* curObjectMesh = curObjectTexturedMeshProperty->mesh;
			camera->renderMesh(curObject, curObjectMesh, getLights());
		}
	}

	// show rendered result
	camera->postrender();

	return true;
}

bool SceneBase::draw()
{
	HDC drawDC = window->getDrawDC();
	//BitBlt(drawDC, 0, 0, window->getResolution()->x, window->getResolution()->y, camera->getDC(), 0, 0, SRCCOPY);
	StretchBlt(drawDC,
		0, 0, window->getResolution()->x, window->getResolution()->y,
		camera->getDC(),
		0, 0, window->getResolution()->x, window->getResolution()->y,
		SRCCOPY);
	return true;
}