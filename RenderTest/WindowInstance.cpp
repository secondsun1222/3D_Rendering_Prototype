#include"WindowInstance.h"

WindowInstance::WindowInstance(MonitorInstance* _monitor, HINSTANCE _hInstance, RECT _position)
	: position(_position)
	, resolution({position.right - position.left, position.bottom - position.top})
	, style(0)
	, monitor(_monitor) {

	LoadStringW(_hInstance, IDC_RENDERTEST, windowClass, MAXLOADSTRING);
	LoadStringW(_hInstance, IDS_APP_TITLE, windowName, MAXLOADSTRING);

	handle = CreateWindowW(windowClass, windowName,
		0,
		-1, -1, 1, 1,
		nullptr, nullptr, _hInstance, nullptr);


	SetWindowLong(handle, GWL_STYLE, 0);
	updateResolution();
	initWindowObjects();

	ShowWindow(handle, SW_SHOW);
}

WindowInstance::WindowInstance(MonitorInstance* _monitor, HINSTANCE _hInstance, RECT _position, UINT _style)
	: position(_position)
	, resolution({ position.right - position.left, position.bottom - position.top })
	, style(_style)
	, monitor(_monitor){

	LoadStringW(_hInstance, IDC_RENDERTEST, windowClass, MAXLOADSTRING);
	LoadStringW(_hInstance, IDS_APP_TITLE, windowName, MAXLOADSTRING);

	handle = CreateWindowW(windowClass, windowName,
		style,
		-1, -1, 1, 1,
		nullptr, nullptr, _hInstance, nullptr);


	SetWindowLong(handle, GWL_STYLE, 0);
	updateResolution();
	initWindowObjects();

	ShowWindow(handle, SW_SHOW);
}

bool WindowInstance::updateResolution() {

	resolution.x = position.right - position.left;
	resolution.y = position.bottom - position.top;

	return SetWindowPos(handle, HWND_TOP,
		position.left, position.top,
		resolution.x, resolution.y, style);
}

bool WindowInstance::initWindowObjects() {

	curDC = GetDC(handle);
	memBM = CreateCompatibleBitmap(curDC, resolution.x, resolution.y);
	memDC = CreateCompatibleDC(curDC);
	HBITMAP oldBM = (HBITMAP)SelectObject(memDC, memBM);
	DeleteObject(oldBM);

	
	HPEN blackBRUSH = (HPEN)GetStockObject(BLACK_BRUSH);
	HPEN oldPEN = (HPEN)SelectObject(curDC, blackBRUSH);
	DeleteObject(oldPEN);
	oldPEN = (HPEN)SelectObject(memDC, blackBRUSH);
	DeleteObject(oldPEN);

	return true;
}

bool WindowInstance::update() {
	BitBlt(curDC, 0, 0, resolution.x, resolution.y, memDC, 0, 0, SRCCOPY);
	Rectangle(memDC, 0, 0, resolution.x, resolution.y);

	return true;
}