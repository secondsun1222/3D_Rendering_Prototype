#pragma once

#include "WindowInstance.h"
#include "SceneBase.h"

class TestScene : public Scene
{
public:
	TestScene(WindowInstance*);
	bool init();
};