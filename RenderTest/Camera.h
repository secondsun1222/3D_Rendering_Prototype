#pragma once
\
#include "pch.h"
#include "Object.h"
#include "Mesh.h"
#include "TexturedMesh.h"
#include "WindowInstance.h"
#include "InputManager.h"
#include "GraphicTools.h"
#include "Properties.h"


class Camera : public Object {
private:
	WindowInstance* window;
	POINT resolution;
	double ppu;			//pixels per unit.. alternative for fov
	double near_d;		//near clipping plane's distance
	double far_d;		//far clipping plane's distance

	double projectionMatrix[4][4];

	HBITMAP camBM;
	HDC		camDC;
	BYTE*	camRGB;
	BITMAPINFOHEADER camBMI;

	float* zBuffer;

public:
	HDC getDC() { return camDC; };

public:
	void openBitmapData();
	void resetBitmapData();
	void applyBitmapData();

	void initBuffers();
	void resetBuffers();
	inline void setRGB(int, int, COLORREF);
	Camera(WindowInstance*, POINT, double, double, double);
	~Camera();

public:
	POINT	textResolution;
	HBITMAP textBM;
	HDC		textDC;
	BYTE*	textRGB;
	BITMAPINFOHEADER textBMI;
	void initTextureData();
	bool readTextureData(WCHAR*);

public:
	void prerender();
	void renderMesh(Object*, Mesh*);
	void renderMesh(Object*, Mesh*, std::vector<Object*>*);
	void renderMesh(Object*, TexturedMesh*, std::vector<Object*>*);
	void postrender();
	virtual bool update();
};