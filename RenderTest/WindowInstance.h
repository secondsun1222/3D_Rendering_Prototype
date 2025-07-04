#pragma once
#include "framework.h"
#include "RenderTest.h"

#include "MonitorInstance.h"
#include"TimeManager.h"

#define MAXLOADSTRING 100

class WindowInstance {
private:
	HWND	handle;
	RECT	position;		//position of the window	{left, top, right, bottom}
	POINT	resolution;		//resolution of the window	{x,y}
	UINT	style;

	WCHAR	windowClass[MAXLOADSTRING] = {};
	WCHAR	windowName[MAXLOADSTRING] = {};

	MonitorInstance* monitor;

	HDC		curDC;
	HBITMAP memBM;
	HDC		memDC;

public:
	WindowInstance(MonitorInstance*, HINSTANCE, RECT);
	WindowInstance(MonitorInstance*, HINSTANCE, RECT, UINT);

	RECT* getPosition() { return &position; }
	POINT* getResolution() { return &resolution; }
	HWND getHandle() { return handle; }
	HDC getDrawDC() { return memDC; }

	bool updateResolution();
	bool initWindowObjects();

	virtual bool update();
};