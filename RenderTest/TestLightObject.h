#pragma once

#include"Object.h"
#include"Properties.h"
#include"TimeManager.h"
#include"InputManager.h"

class TestLightObject : public Object
{
public:
	LightProperty* lightProp;
	TestLightObject();
public:
	bool init();
	bool update();
};