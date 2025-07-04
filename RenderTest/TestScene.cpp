#include "TestScene.h"
#include "TestObject.h"
#include "TestLightObject.h"

TestScene::TestScene(WindowInstance* _window)
	:Scene(_window)
{
}

bool TestScene::init()
{
	root = new Object();


	TestObject* obj1 = new TestObject();
	TestObject* obj2 = new TestObject();
	{
		std::vector<Vec3> vertexList = {
			{5.0,5.0,5.0},
			{5.0,5.0,-5.0},
			{5.0,-5.0,5.0},
			{5.0,-5.0,-5.0},
			{-5.0,5.0,5.0},
			{-5.0,5.0,-5.0},
			{-5.0,-5.0,5.0},
			{-5.0,-5.0,-5.0},
		};
		std::vector<Line> lineList = {};
		std::vector<UVFace> UVFaceList = {
			{0,		0,2,1,		{180,180},{180,0},{0,180}},
			{0,		1,2,3,		{0,180},{180,0},{0,0}},
			
			{0,		0,1,4,		{180,180},{180,0},{0,180}},
			{0,		4,1,5,		{0,180},{180,0},{0,0}},
			
			//back
			{0,		0,4,2,		{180,180},{180,0},{0,180}},
			{0,		2,4,6,		{0,180},{180,0},{0,0}},

			{0,		7,6,5,		{180,180},{180,0},{0,180}},
			{0,		5,6,4,		{0,180},{180,0},{0,0}},

			//down
			{0,		7,3,6,		{180,180},{180,0},{0,180}},
			{0,		6,3,2,		{0,180},{180,0},{0,0}},

			//front
			{0,		7,5,3,		{180,180},{180,0},{0,180}},
			{0,		3,5,1,		{0,180},{180,0},{0,0}},
			
		};
		std::vector<WCHAR*> textureList = {
			(WCHAR*)L".\\Resource\\metal.bmp"
		};

		TexturedMesh* textMesh = new TexturedMesh(vertexList, lineList, UVFaceList, textureList);
		TexturedMeshProperty* textMeshProp = new TexturedMeshProperty(textMesh);
		obj1->position = { 0,0,0 };
		obj1->addProperty(textMeshProp);
		obj1->setParent(root);

		//obj2->addProperty(textMeshProp);
		//obj2->setParent(obj1);
		//obj2->position = { 10,10,10 };
	}
	{
		TestLightObject* lightObj = new TestLightObject();
		lightObj->position = { 0.0,15.0,-10.0 };
		LightProperty* lightProp = new LightProperty(LightType::POSITIONAL, 0.0, RGB(255, 0, 0));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	{
		TestLightObject* lightObj = new TestLightObject();
		lightObj->position = { -3.0,0.0,-10.0 };
		LightProperty* lightProp = new LightProperty(LightType::POSITIONAL, 20.0, RGB(255, 255, 255));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	
	{
		Object* lightObj = new Object();
		LightProperty* lightProp = new LightProperty(LightType::UNIVERSAL, 0.1, RGB(255, 255, 255));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}




	camera = new Camera(window, *(window->getResolution()), 100.0, 5.0, 100.0);
	camera->setParent(root);

	camera->position = { 0.0,0.0,-60.0 };
	Quaternion q = makeQuaternion(-0.0, { 1,0,0 });
	camera->rotation = Quaternion::multiply(camera->rotation, q);

	fetchLights();

	return false;
}
