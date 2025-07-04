#pragma once
#include "pch.h"
#include "Object.h"
#include "Camera.h"
#include "WindowInstance.h"
#include "Properties.h"
#include "LightManager.h"

class SceneBase {
protected:
	WindowInstance* window;
	Object* root;
	Camera* camera;
	LightManager* lightManager;

public:
	void addLightObject(Object*);
	void removeLightObject(Object*);
	std::vector<Object*>* getLights();
	void fetchLights();

public:
	SceneBase(WindowInstance*, Object*, Camera*);
	SceneBase(WindowInstance*);

	virtual bool init();
	bool update();
	bool render();
	virtual bool draw();
	// intended flow: init -> loop(update -> render -> draw)
};

typedef SceneBase Scene;