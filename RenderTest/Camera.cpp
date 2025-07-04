#include "Camera.h"

Camera::Camera(WindowInstance* _window, POINT _resolution, double _ppu, double _near_d, double _far_d)
	:Object()
	,window(_window)
	,resolution(_resolution)
	,ppu(_ppu)
	,near_d(_near_d)
	,far_d(_far_d)
	,projectionMatrix()
{
	projectionMatrix[0][0] = 2 * ppu * near_d / resolution.x;
	projectionMatrix[1][1] = 2 * ppu * near_d / resolution.y;

	projectionMatrix[2][2] = far_d				/ (far_d - near_d);
	projectionMatrix[3][2] = -far_d * near_d	/ (far_d - near_d);
	projectionMatrix[2][3] = 1;

	HDC windowDC = GetDC(window->getHandle());
	camBM = CreateCompatibleBitmap(windowDC, resolution.x, resolution.y);
	camDC = CreateCompatibleDC(windowDC);
	HBITMAP oldBM = (HBITMAP)SelectObject(camDC, camBM);
	DeleteObject(oldBM);

	HPEN blackBRUSH = (HPEN)GetStockObject(BLACK_BRUSH);
	HPEN oldPEN = (HPEN)SelectObject(camDC, blackBRUSH);
	DeleteObject(oldPEN);

	openBitmapData();
	initBuffers();
	resetBuffers();
	initTextureData();

	INSTANCE(InputManager)->FixCursor({ window->getPosition()->left + resolution.x / 2, window->getPosition()->top + resolution.y / 2 });
	ShowCursor(false);
}

Camera::~Camera() {
	free(camRGB);
	free(zBuffer);
}

void Camera::openBitmapData() {

	camBMI = { 0 };
	camBMI.biSize = sizeof(BITMAPINFOHEADER);
	camBMI.biPlanes = 1;
	camBMI.biBitCount = 32;
	camBMI.biWidth = resolution.x;
	camBMI.biHeight = resolution.y;
	camBMI.biCompression = BI_RGB;
	camBMI.biSizeImage = 3 * resolution.x * resolution.y;

	camRGB = (BYTE*)malloc(4 * resolution.x * resolution.y);

	GetDIBits(camDC, camBM, 0, resolution.y, camRGB, (BITMAPINFO*)&camBMI, DIB_RGB_COLORS);
}

void Camera::resetBitmapData() {
	Rectangle(camDC, 0, 0, resolution.x, resolution.y);
	GetDIBits(camDC, camBM, 0, resolution.y, camRGB, (BITMAPINFO*)&camBMI, DIB_RGB_COLORS);
}

void Camera::applyBitmapData() {
	SetDIBits(camDC, camBM, 0, resolution.y, camRGB, (BITMAPINFO*)&camBMI, DIB_RGB_COLORS);
}

void Camera::initBuffers() {
	zBuffer = (float*)malloc(sizeof(float) * resolution.x * resolution.y);
}

void Camera::resetBuffers() {
	memset(zBuffer, 0, sizeof(float) * resolution.x * resolution.y);
}

void Camera::initTextureData()
{
	HDC windowDC = GetDC(window->getHandle());
	textDC = CreateCompatibleDC(windowDC);

	textBMI = { 0 };
	textBMI.biSize = sizeof(BITMAPINFOHEADER);
	textBMI.biPlanes = 1;
	textBMI.biBitCount = 32;
	textBMI.biWidth = 1;
	textBMI.biHeight = 1;
	textBMI.biCompression = BI_RGB;
	textBMI.biSizeImage = 3 * 1;

	textRGB = (BYTE*)malloc(4 * 1);
}


bool Camera::readTextureData(WCHAR* path) {

	textBM = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	if (textBM == NULL) {
		MessageBox(window->getHandle(), L"Texture Load Failed", L"Erorr", MB_OK);
		return false;
	}

	HBITMAP oldBM = (HBITMAP)SelectObject(textDC, textBM);
	DeleteObject(oldBM);

	BITMAP BM;
	GetObject(textBM, sizeof(BM), &BM);

	textResolution.x = BM.bmWidth;
	textResolution.y = BM.bmHeight;

	textBMI.biWidth = textResolution.x;
	textBMI.biHeight = textResolution.y;

	free(textRGB);
	textRGB = (BYTE*)malloc(4 * textResolution.x * textResolution.y);

	GetDIBits(textDC, textBM, 0, textResolution.y, textRGB, (BITMAPINFO*)&textBMI, DIB_RGB_COLORS);
	return true;
}

void Camera::prerender() {
	resetBitmapData();
	resetBuffers();
}

void Camera::renderMesh(Object* object, Mesh* mesh)	// render without light
{
	std::vector<Vec3> vertexTransformed;
	std::vector<Vec3> vertexPixel;

	for (std::vector<Vec3>::iterator vp = mesh->vertexList.begin(); vp != mesh->vertexList.end(); vp++) {

		//calculate the relative position of vp to the camera
		Vec3 v = *vp;
		v = Quaternion::rotate(object->rotation, v);
		v += object->position - this->position;
		v = Quaternion::rotate(this->rotation.inverse(), v);
		
		Vec3 vTransform;
		vTransform.x = v.x * projectionMatrix[0][0];
		vTransform.y = v.y * projectionMatrix[1][1];
		vTransform.z = v.z * projectionMatrix[2][2] + 1 * projectionMatrix[3][2];

		double w = v.z * projectionMatrix[2][3];
		vTransform /= w;

		vertexTransformed.push_back(vTransform);

		Vec3 vPixel;
		vPixel.x = (vTransform.x + 0.5) * resolution.x;
		vPixel.y = (vTransform.y + 0.5) * resolution.y;
		vPixel.z = vTransform.z;

		vertexPixel.push_back(vPixel);
	}

	// vertex debugging
	/*
	for (std::vector<Vec3>::iterator vp = vertexTransformed.begin(); vp != vertexTransformed.end(); vp++) {

		if (vp->z <= 0 || 1 <= vp->z)continue;

		int pixelX = (int)((vp->x + 0.5) * resolution.x);
		int pixelY = (int)((vp->y + 0.5) * resolution.y);
		int distance = (int)(vp->z * 255);

		for (int py = pixelY - 5; py < pixelY + 5; py++) {
			for (int px = pixelX - 5; px < pixelX + 5; px++) {
				setRGB(py, px, RGB(255 - distance, 255 - distance, 255 - distance));
			}
		}

	}*/

	// render mesh with pseudo scan line
	/*
	for (std::vector<Face>::iterator fp = mesh->faceList.begin(); fp != mesh->faceList.end(); fp++) {

		Vec3 vArr[] = { vertexPixel[fp->v1], vertexPixel[fp->v2], vertexPixel[fp->v3] };

		bool doRender = true;
		//check if any vertex is out of camera's near or far plane
		for (int i = 0; i < sizeof(vArr) / sizeof(Vec3); i++) {
			if (vArr[i].z < 0 || 1 < vArr[i].z) {
				doRender = false;
				break;
			}
		}
		if (!doRender)continue;


		std::sort(vArr, vArr + 3, SortVec3ByY);

		Vec3 boundMin,
			boundMax;
		BoundRectVec3(vArr, 3, boundMin, boundMax);

		// set rectangle boundary to perform psuedo scanline algorithm below
		int boundLeft = (int)boundMin.x,
			boundRight = (int)boundMax.x,

			boundBottom = max((int)boundMin.y, 0),
			boundMid = round((int)vArr[1].y, 0, resolution.y - 1),
			boundTop = min((int)boundMax.y, resolution.y - 1);


		double zAvg = (vArr[0].z + vArr[1].z + vArr[2].z) / 3;
		int dist = 255 - zAvg * 255;
		COLORREF colortemp = RGB(dist, dist, dist);

		// pseudo scanline algorithm
		// bounding for gap inefficient
		// ???
		// accurate to the pixel

		double	xInter1, xInter2;	// intersections of edges and scan line
		double increm1, increm2;	// increments of intersections

		// bottom triangle
		if (boundBottom != boundMid) {	// skip when triangle is flat

			increm1 = (vArr[1].x - vArr[0].x) / (vArr[1].y - vArr[0].y);
			increm2 = (vArr[2].x - vArr[0].x) / (vArr[2].y - vArr[0].y);

			// ensure xInter1 <= xInter2
			if (increm1 > increm2) {	// swap increms
				double temp = increm1;
				increm1 = increm2;
				increm2 = temp;
			}

			// scan?
			for (int py = boundBottom; py < boundMid; py++) {
				double gap = (((double)py) - vArr[0].y);
				if (gap < 0)gap = 0;
				if (gap > vArr[1].y - vArr[0].y)gap = vArr[1].y - vArr[0].y;

				xInter1 = round(vArr[0].x + gap * increm1, 0, resolution.x - 1);
				xInter2 = round(vArr[0].x + gap * increm2, 0, resolution.x - 1);

				for (int px = (int)xInter1; px < (int)xInter2; px++) {
					if (zBuffer[py * resolution.x + px] <= 0.0f ||
						zBuffer[py * resolution.x + px] > zAvg) {
						zBuffer[py * resolution.x + px] = zAvg;
						setRGB(py, px, colortemp);
					}
				}
			}
		}
		// top triangle
		if (boundTop != boundMid) {	// skip when triangle is flat

			increm1 = (vArr[1].x - vArr[2].x) / (vArr[1].y - vArr[2].y);
			increm2 = (vArr[0].x - vArr[2].x) / (vArr[0].y - vArr[2].y);

			// ensure xInter1 >= xInter2
			if (increm1 < increm2) {	// swap increms
				double temp = increm1;
				increm1 = increm2;
				increm2 = temp;
			}

			// scan?
			for (int py = boundTop; py >= boundMid; py--) {
				double gap = (((double)py) - vArr[2].y);
				if (gap > 0)gap = 0;
				if (gap < vArr[1].y - vArr[2].y)gap = vArr[1].y - vArr[2].y;

				xInter1 = round(vArr[2].x + gap * increm1, 0, resolution.x - 1);
				xInter2 = round(vArr[2].x + gap * increm2, 0, resolution.x - 1);

				for (int px = (int)xInter1; px < (int)xInter2; px++) {
					if (zBuffer[py * resolution.x + px] <= 0.0f ||
						zBuffer[py * resolution.x + px] > zAvg) {
						zBuffer[py * resolution.x + px] = zAvg;
						setRGB(py, px, colortemp);
					}
				}
			}
		}

	}
	*/

	// render mesh
	for (std::vector<Face>::iterator fp = mesh->faceList.begin(); fp != mesh->faceList.end(); fp++) {

		Vec3 vArr[] = { vertexPixel[fp->v1], vertexPixel[fp->v2], vertexPixel[fp->v3] };

		//check if any vertex is out of camera's near or far plane
		bool doRender = true;
		for (int i = 0; i < sizeof(vArr) / sizeof(Vec3); i++) {
			if (vArr[i].z < 0 || 1 < vArr[i].z) {
				doRender = false;
				break;
			}
		}
		if (!doRender)continue;


		std::sort(vArr, vArr + 3, SortVec3ByY);

		Vec3 boundMin,
			boundMax;
		BoundRectVec3(vArr, 3, boundMin, boundMax);

		// set rectangle boundary to perform psuedo scanline algorithm below
		int boundLeft = max((int)boundMin.x, 0),
			boundRight = min((int)boundMax.x, resolution.x - 1),

			boundBottom = max((int)boundMin.y, 0),
			boundTop = min((int)boundMax.y, resolution.y - 1);

		Vec3 d01 = vArr[1] - vArr[0];
		Vec3 d02 = vArr[2] - vArr[0];

		if (d01.x * d02.y == d02.x * d01.y)continue;	// vertices are on same line

		for (int py = boundBottom; py <= boundTop; py++) {
			for (int px = boundLeft; px <= boundRight; px++) {
				double w0, w1, w2;	// weight of each vertex for the pixel
				Vec3 d0p = Vec3{ (double)(px),(double)(py) } - vArr[0];	// vector from first vertex of triangle to pixel

				// from solving the equations
				//		w1 * d01.x + w2 * d02.x = d0p.x
				//		w1 * d01.y + w2 * d02.y = d0p.y
				//		w0 + w1 + w2 = 1
				w1 = (d0p.x * d02.y - d02.x * d0p.y) / (d01.x * d02.y - d02.x * d01.y);
				w2 = (d0p.x * d01.y - d01.x * d0p.y) / (d02.x * d01.y - d01.x * d02.y);
				w0 = 1 - (w1 + w2);

				if (w0 < 0 || w1 < 0 || w2 < 0) continue;	// pixel is not on triangle

				double pz = vArr[0].z * w0 + vArr[1].z * w1 + vArr[2].z * w2;

				if (zBuffer[py * resolution.x + px] <= 0.0f ||
					zBuffer[py * resolution.x + px] > pz) {
					zBuffer[py * resolution.x + px] = pz;
					setRGB(py, px,
						RGB(
							(1 - pz) * 255,
							(1 - pz) * 255,
							(1 - pz) * 255
						)
					);
				}
			}
		}

	}
}

void Camera::renderMesh(Object* object, Mesh* mesh, std::vector<Object*>* lights)
{
	std::vector<Vec3> vertexRelat;	// relative position to the camera of each vertex
	std::vector<Vec3> vertexTrans;	// transformed via the projection matrix
	std::vector<Vec3> vertexPixel;	// x and y stretched to fall into the pixels of the screen

	for (std::vector<Vec3>::iterator vp = mesh->vertexList.begin(); vp != mesh->vertexList.end(); vp++) {

		//calculate the relative position of vp to the camera
		Vec3 vRelat = *vp;
		vRelat = Quaternion::rotate(object->rotation, vRelat);
		vRelat += object->position - this->position;
		vRelat = Quaternion::rotate(this->rotation.inverse(), vRelat);

		vertexRelat.push_back(vRelat);

		Vec3 vTransform;
		vTransform.x = vRelat.x * projectionMatrix[0][0];
		vTransform.y = vRelat.y * projectionMatrix[1][1];
		vTransform.z = vRelat.z * projectionMatrix[2][2] + 1 * projectionMatrix[3][2];

		double w = vRelat.z * projectionMatrix[2][3];
		vTransform /= w;

		vertexTrans.push_back(vTransform);

		Vec3 vPixel;
		vPixel.x = (vTransform.x + 0.5) * resolution.x;
		vPixel.y = (vTransform.y + 0.5) * resolution.y;
		vPixel.z = vTransform.z;

		vertexPixel.push_back(vPixel);
	}

	// render mesh with light
	for (std::vector<Face>::iterator fp = mesh->faceList.begin(); fp != mesh->faceList.end(); fp++) {

		Vec3 vArrRelat[] = { vertexRelat[fp->v1], vertexRelat[fp->v2], vertexRelat[fp->v3] };
		Vec3 vArrTrans[] = { vertexTrans[fp->v1], vertexTrans[fp->v2], vertexTrans[fp->v3] };
		Vec3 vArrPixel[] = { vertexPixel[fp->v1], vertexPixel[fp->v2], vertexPixel[fp->v3] };
		Vec3 vNormal = Vec3::cross(vArrRelat[1] - vArrRelat[0], vArrRelat[2] - vArrRelat[0]).unit();

		//check if any vertex is out of camera's near or far plane
		bool doRender = true;
		for (int i = 0; i < sizeof(vArrPixel) / sizeof(Vec3); i++) {
			if (vArrPixel[i].z < 0 || 1 < vArrPixel[i].z) {
				doRender = false;
				break;
			}
		}
		if (!doRender)
			continue;


		Vec3 boundMin, boundMax;
		BoundRectVec3(vArrPixel, 3, boundMin, boundMax);

		// set rectangle boundary
		int boundLeft = max((int)boundMin.x, 0),
			boundRight = min((int)boundMax.x, resolution.x - 1),

			boundBottom = max((int)boundMin.y, 0),
			boundTop = min((int)boundMax.y, resolution.y - 1);

		Vec3 d01Relat = vArrRelat[1] - vArrRelat[0];
		Vec3 d02Relat = vArrRelat[2] - vArrRelat[0];


		for (int py = boundBottom; py <= boundTop; py++) {
			for (int px = boundLeft; px <= boundRight; px++) {
				double w0, w1, w2;	// weight of each vertex for the pixel
				Vec3 pTrans = Vec3{
					px / (double)resolution.x - 0.5,
					py / (double)resolution.y - 0.5
				}; // vector from first vertex of triangle to pixel

				// pixel's direction unit in relative dimension
				Vec3 vpUnitRelat;
				vpUnitRelat.z = projectionMatrix[3][2] / (pTrans.z * projectionMatrix[2][3] - projectionMatrix[2][2]);
				vpUnitRelat.x = pTrans.x * vpUnitRelat.z * projectionMatrix[2][3] / projectionMatrix[0][0];
				vpUnitRelat.y = pTrans.y * vpUnitRelat.z * projectionMatrix[2][3] / projectionMatrix[1][1];

				vpUnitRelat /= vpUnitRelat.z;

				Vec3 d01Proj = d01Relat + vpUnitRelat * (vArrRelat[0].z - vArrRelat[1].z);
				Vec3 d02Proj = d02Relat + vpUnitRelat * (vArrRelat[0].z - vArrRelat[2].z);
				Vec3 d0pProj = -vArrRelat[0] + vpUnitRelat * vArrRelat[0].z;

				// from solving the equations
				//		w1 * d01.x + w2 * d02.x = d0p.x
				//		w1 * d01.y + w2 * d02.y = d0p.y
				//		w0 + w1 + w2 = 1

				w1 = (d0pProj.x * d02Proj.y - d02Proj.x * d0pProj.y) / (d01Proj.x * d02Proj.y - d02Proj.x * d01Proj.y);
				w2 = (d0pProj.x * d01Proj.y - d01Proj.x * d0pProj.y) / (d02Proj.x * d01Proj.y - d01Proj.x * d02Proj.y);
				w0 = 1.0 - (w1 + w2);

				if (w0 < 0 || w1 < 0 || w2 < 0) continue;	// pixel is not on triangle


				// calculate pixel's intersection with the triangle, and the transformed z depth
				Vec3 pRelat = vArrRelat[0] * w0 + vArrRelat[1] * w1 + vArrRelat[2] * w2;

				double pz = pRelat.z * projectionMatrix[2][2] + 1 * projectionMatrix[3][2];
				double w = pRelat.z * projectionMatrix[2][3];
				pz /= w;
				// calculate pixel's intersection with the triangle, and the transformed z depth

				// zBuffer check
				if (zBuffer[py * resolution.x + px] > 0.0f &&
					zBuffer[py * resolution.x + px] < pz)
					continue;	// failed zBuffer check
				// zBuffer check

				// calculate color with lights
				int r = 0, g = 0, b = 0;
				for (std::vector<Object*>::iterator it = lights->begin(); it != lights->end(); it++) {
					LightProperty* lightProp = (LightProperty*)(*it)->findProperty(PropertyType::LIGHT);

					switch (lightProp->type) {
					case LightType::POSITIONAL:
					{
						Vec3 pLightRelat = (*it)->position - this->position;	// position of light seen by the camera
						pLightRelat = Quaternion::rotate(this->rotation.inverse(), pLightRelat);

						double vlpMag = (pRelat - pLightRelat).magnitude();
						Vec3 vlp = (pRelat - pLightRelat).unit();	// direction vector from pixel to light

						double lightQuantity = max(Vec3::dot(vNormal, vlp), 0) / (vlpMag * vlpMag);
						r += lightQuantity * lightProp->brightness * RGB_R(lightProp->color);
						g += lightQuantity * lightProp->brightness * RGB_G(lightProp->color);
						b += lightQuantity * lightProp->brightness * RGB_B(lightProp->color);
						break;
					}
					case LightType::UNIVERSAL:
					{
						r += lightProp->brightness * RGB_R(lightProp->color);
						g += lightProp->brightness * RGB_G(lightProp->color);
						b += lightProp->brightness * RGB_B(lightProp->color);
						break;
					}
					}
				}

				r = min(r, 255);
				g = min(g, 255);
				b = min(b, 255);

				zBuffer[py * resolution.x + px] = pz;
				setRGB(py, px, RGB(r,g,b));
			}
		}

	}
}

void Camera::renderMesh(Object* object, TexturedMesh* mesh, std::vector<Object*>* lights) {

	std::vector<Vec3> vertexRelat;	// relative position to the camera of each vertex
	std::vector<Vec3> vertexTrans;	// transformed via the projection matrix
	std::vector<Vec3> vertexPixel;	// x and y stretched to fall into the pixels of the screen

	for (std::vector<Vec3>::iterator vp = mesh->vertexList.begin(); vp != mesh->vertexList.end(); vp++) {

		// calculate the relative position of vp to the camera
		Vec3 vRelat = *vp;
		vRelat = Quaternion::rotate(object->originRotation, vRelat);
		vRelat += object->originPosition - this->originPosition;
		vRelat = Quaternion::rotate(this->originRotation.inverse(), vRelat);

		vertexRelat.push_back(vRelat);

		// transformed through the projection matrix
		Vec3 vTransform;
		vTransform.x = vRelat.x * projectionMatrix[0][0];
		vTransform.y = vRelat.y * projectionMatrix[1][1];
		vTransform.z = vRelat.z * projectionMatrix[2][2] + 1 * projectionMatrix[3][2];

		double w = vRelat.z * projectionMatrix[2][3];
		vTransform /= w;

		vertexTrans.push_back(vTransform);

		// stretch x and y s.t. they represent pixels on screen
		Vec3 vPixel;
		vPixel.x = (vTransform.x + 0.5) * resolution.x;
		vPixel.y = (vTransform.y + 0.5) * resolution.y;
		vPixel.z = vTransform.z;

		vertexPixel.push_back(vPixel);
	}

	std::sort(mesh->UVFaceList.begin(), mesh->UVFaceList.end(),
		[](UVFace face1, UVFace face2) {
			return face1.texture < face2.texture;
		}
	);

	int curTexture = -1;

	// render textured mesh with light
	for (std::vector<UVFace>::iterator fp = mesh->UVFaceList.begin(); fp != mesh->UVFaceList.end(); fp++) {

		if (curTexture != fp->texture) {
			curTexture = fp->texture;
			bool readTexture = readTextureData(mesh->textureList[fp->texture]);
		}

		Vec3 vArrRelat[] = { vertexRelat[fp->v1], vertexRelat[fp->v2], vertexRelat[fp->v3] };
		Vec3 vArrTrans[] = { vertexTrans[fp->v1], vertexTrans[fp->v2], vertexTrans[fp->v3] };
		Vec3 vArrPixel[] = { vertexPixel[fp->v1], vertexPixel[fp->v2], vertexPixel[fp->v3] };
		Vec3 vNormal = Vec3::cross(vArrRelat[1] - vArrRelat[0], vArrRelat[2] - vArrRelat[0]).unit();

		//check if any vertex is out of camera's near or far plane
		bool doRender = true;
		for (int i = 0; i < sizeof(vArrPixel) / sizeof(Vec3); i++) {
			if (vArrPixel[i].z < 0 || 1 < vArrPixel[i].z) {
				doRender = false;
				break;
			}
		}
		if (!doRender)continue;


		Vec3 boundMin, boundMax;
		BoundRectVec3(vArrPixel, 3, boundMin, boundMax);

		// set rectangle boundary
		int boundLeft = max((int)boundMin.x, 0),
			boundRight = min((int)boundMax.x, resolution.x - 1),

			boundBottom = max((int)boundMin.y, 0),
			boundTop = min((int)boundMax.y, resolution.y - 1);

		Vec3 d01Relat = vArrRelat[1] - vArrRelat[0];
		Vec3 d02Relat = vArrRelat[2] - vArrRelat[0];

		// go through individual pixels
		for (int py = boundBottom; py <= boundTop; py++) {
			for (int px = boundLeft; px <= boundRight; px++) {
				double w0, w1, w2;	// weight of each vertex for the pixel

				Vec3 pTrans = Vec3{
					px / (double)resolution.x - 0.5,
					py / (double)resolution.y - 0.5
				}; // vector from first vertex of triangle to pixel

				// pixel's direction unit in relative dimension
				Vec3 vpUnitRelat;
				vpUnitRelat.z = projectionMatrix[3][2] / (pTrans.z * projectionMatrix[2][3] - projectionMatrix[2][2]);
				vpUnitRelat.x = pTrans.x * vpUnitRelat.z * projectionMatrix[2][3] / projectionMatrix[0][0];
				vpUnitRelat.y = pTrans.y * vpUnitRelat.z * projectionMatrix[2][3] / projectionMatrix[1][1];

				vpUnitRelat /= vpUnitRelat.z;

				Vec3 d01Proj = d01Relat + vpUnitRelat * (vArrRelat[0].z - vArrRelat[1].z);
				Vec3 d02Proj = d02Relat + vpUnitRelat * (vArrRelat[0].z - vArrRelat[2].z);
				Vec3 d0pProj = -vArrRelat[0] + vpUnitRelat * vArrRelat[0].z;

				// from solving the equations
				//		w1 * d01.x + w2 * d02.x = d0p.x
				//		w1 * d01.y + w2 * d02.y = d0p.y
				//		w0 + w1 + w2 = 1
				w1 = (d0pProj.x * d02Proj.y - d02Proj.x * d0pProj.y) / (d01Proj.x * d02Proj.y - d02Proj.x * d01Proj.y);
				w2 = (d0pProj.x * d01Proj.y - d01Proj.x * d0pProj.y) / (d02Proj.x * d01Proj.y - d01Proj.x * d02Proj.y);
				w0 = 1.0 - (w1 + w2);

				if (w0 < 0 || w1 < 0 || w2 < 0) {
					continue;	// pixel is not on triangle
				}


				// calculate pixel's intersection with the triangle, and the transformed z depth
				Vec3 pRelat = vArrRelat[0] * w0 + vArrRelat[1] * w1 + vArrRelat[2] * w2;

				double pz = pRelat.z * projectionMatrix[2][2] + 1 * projectionMatrix[3][2];
				double w = pRelat.z * projectionMatrix[2][3];
				pz /= w;
				// calculate pixel's intersection with the triangle, and the transformed z depth

				// zBuffer check
				if (zBuffer[py * resolution.x + px] > 0.0f &&
					zBuffer[py * resolution.x + px] < pz)
					continue;	// failed zBuffer check
				// zBuffer check

				Vec2 pTexture = fp->v1UV * w0 + fp->v2UV * w1 + fp->v3UV * w2;	// position of the pixel on texture
				int		textX = (int)pTexture.x;
				int		textY = (int)pTexture.y;
				double textR = textRGB[4 * (textResolution.x * (textResolution.y - textY - 1) + textX) + 0];
				double textG = textRGB[4 * (textResolution.x * (textResolution.y - textY - 1) + textX) + 1];
				double textB = textRGB[4 * (textResolution.x * (textResolution.y - textY - 1) + textX) + 2];

				// calculate color with lights
				int r = 0, g = 0, b = 0;
				for (std::vector<Object*>::iterator lp = lights->begin(); lp != lights->end(); lp++) {
					LightProperty* lightProp = (LightProperty*)(*lp)->findProperty(PropertyType::LIGHT);

					switch (lightProp->type) {
					case LightType::POSITIONAL:
					{
						Vec3 pLightRelat = (*lp)->originPosition - this->originPosition;	// position of light seen by the camera
						pLightRelat = Quaternion::rotate(this->originRotation.inverse(), pLightRelat);

						double vplMag = (pLightRelat - pRelat).magnitude();
						Vec3 vpl = (pLightRelat - pRelat).unit();	// direction vector from pixel to light
						
						

						Vec3 vHalf = (-pRelat.unit() + vpl).unit();
						double lightSpecular = pow(max(Vec3::dot(vHalf, vNormal), 0),4.0) 
							* lightProp->brightness
							/ (vplMag * vplMag);
						double lightDiffuse = max(Vec3::dot(vNormal, vpl), 0) 
							*lightProp->brightness
							/ (vplMag * vplMag);

						r += lightSpecular * RGB_R(lightProp->color) + lightDiffuse * textR * RGB_R(lightProp->color) / 255.0;
						g += lightSpecular * RGB_G(lightProp->color) + lightDiffuse * textG * RGB_R(lightProp->color) / 255.0;
						b += lightSpecular * RGB_B(lightProp->color) + lightDiffuse * textB * RGB_R(lightProp->color) / 255.0;
						break;
					}
					case LightType::UNIVERSAL:
					{
						r += lightProp->brightness * RGB_R(lightProp->color) * textR / 255;
						g += lightProp->brightness * RGB_G(lightProp->color) * textG / 255;
						b += lightProp->brightness * RGB_B(lightProp->color) * textB / 255;
						break;
					}
					}
				}

				r = min(r, 255);
				g = min(g, 255);
				b = min(b, 255);

				zBuffer[py * resolution.x + px] = pz;
				setRGB(py, px, RGB(r, g, b));
			}
		}
		// go through individual pixels

	}
	// render textured mesh with light

	// render direct light
	for (std::vector<Object*>::iterator lp = lights->begin(); lp != lights->end(); lp++) {
		// todo
		continue;
	}
	// render direct light
}

void Camera::postrender() {
	applyBitmapData();
}

inline void Camera::setRGB(int y, int x, COLORREF color) {
	if (0 <= x && x < resolution.x && 0 <= y && y < resolution.y) {
		camRGB[4 * (resolution.x * (resolution.y - y - 1) + x)] = RGB_B(color);
		camRGB[4 * (resolution.x * (resolution.y - y - 1) + x) + 1] = RGB_G(color);
		camRGB[4 * (resolution.x * (resolution.y - y - 1) + x) + 2] = RGB_R(color);
	}
}

bool Camera::update()
{
	/*
	double deltaTime = INSTANCE(TimeManager)->DeltaTime;
	POINT cursorMovement = INSTANCE(InputManager)->GetCursorMovement();


	Quaternion cursorRotationX = makeQuaternion(-cursorMovement.x / 10.0 * deltaTime, Vec3{ 0,1,0 });
	rotation = Quaternion::multiply(cursorRotationX, rotation);

	Vec3 dir = Quaternion::rotate(rotation, { 1,0,0 });
	Quaternion cursorRotationY = makeQuaternion(-cursorMovement.y / 10.0 * deltaTime, Vec3{ dir.x,0.0,dir.z }.unit());
	rotation = Quaternion::multiply(cursorRotationY, rotation);

	Vec3 dirWASD = {};

	double speed = 5.0;
	if (INSTANCE(InputManager)->GetKeyState(KEY::W) == KEY_STATE::HOLD) dirWASD.z += 1.0;
	if (INSTANCE(InputManager)->GetKeyState(KEY::A) == KEY_STATE::HOLD) dirWASD.x -= 1.0;
	if (INSTANCE(InputManager)->GetKeyState(KEY::S) == KEY_STATE::HOLD) dirWASD.z -= 1.0;
	if (INSTANCE(InputManager)->GetKeyState(KEY::D) == KEY_STATE::HOLD) dirWASD.x += 1.0;
	if(dirWASD.magnitude() != 0.0)dirWASD = dirWASD.unit();

	position += Quaternion::rotate(rotation, dirWASD * speed * deltaTime);
	*/
	
	return true;
}