#include "LightManager.h"



void LightManager::addLight(Object* lightObject)
{
	LightProperty* lightProp = (LightProperty*)lightObject->findProperty(PropertyType::LIGHT);
	if (lightProp == nullptr) return;	// object does not contain light property

	auto it = std::find(lights.begin(), lights.end(), lightObject);
	if (it == lights.end()) lights.push_back(lightObject);	//object is not already in manager
}

void LightManager::removeLight(Object* lightObject)
{
	LightProperty* lightProp = (LightProperty*)lightObject->findProperty(PropertyType::LIGHT);
	if (lightProp == nullptr) return;	// object does not contain light property

	auto it = std::find(lights.begin(), lights.end(), lightObject);
	if (it == lights.end()) return;	//object is not in manager

	lights.erase(it);
}

std::vector<Object*>* LightManager::getLights()
{
	return &lights;
}


LightManager::LightManager() {

}