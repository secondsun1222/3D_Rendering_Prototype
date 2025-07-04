#pragma once

#include"Object.h"

class TestLightObject : public Object
{
public:
	TestLightObject();
public:
	bool init();
	bool update();
};