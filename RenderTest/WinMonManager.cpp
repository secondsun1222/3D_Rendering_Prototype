#include "WinMonManager.h"

WinMonManager::WinMonManager(HINSTANCE _handle)
	: handle(_handle) {

	MonitorInstance* mInstance = new MonitorInstance();
	monitorMain = mInstance;
}

WindowInstance* WinMonManager::makeNewWindow(RECT _resolution) {

	WindowInstance* newWindow = new WindowInstance(monitorMain, handle, _resolution);

	windowInstances.push_back(newWindow);

	return newWindow;
}

WindowInstance* WinMonManager::makeNewWindow(RECT _resolution, UINT _style) {

	WindowInstance* newWindow = new WindowInstance(monitorMain, handle, _resolution, _style);

	windowInstances.push_back(newWindow);

	return newWindow;
}

bool WinMonManager::update() {

	for (auto it : windowInstances) {
		if (!it->update()) return false;
	}
	return true;
}