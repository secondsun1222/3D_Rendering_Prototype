#pragma once
#include "pch.h"
#include "Object.h"
#include "Properties.h"

class LightManager {
private:
	std::vector<Object*> lights;

public:
	void addLight(Object* lightObject);
	void removeLight(Object* lightObject);

	std::vector<Object*>* getLights();
	
	LightManager();
};