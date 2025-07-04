#pragma once
#include "framework.h"
#include "WindowInstance.h"
#include "MonitorInstance.h"

class WinMonManager {

private:
	HINSTANCE handle;
	std::vector<WindowInstance*> windowInstances;
	MonitorInstance* monitorMain;

public:
	WinMonManager(HINSTANCE);

	WindowInstance* makeNewWindow(RECT);
	WindowInstance* makeNewWindow(RECT, UINT);

	bool update();
};