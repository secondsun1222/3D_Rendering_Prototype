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
		std::vector<Material> materialList = {
			{(WCHAR*)L".\\Resource\\Images\\Texture\\metal.bmp", 1.0},
		};
		std::vector<Material> materialList2 = {
			{(WCHAR*)L".\\Resource\\Images\\Texture\\wood.bmp", 0.3},
		};

		TexturedMesh* textMesh = new TexturedMesh(vertexList, lineList, UVFaceList, materialList);
		TexturedMeshProperty* textMeshProp = new TexturedMeshProperty(textMesh);
		TexturedMesh* textMesh2 = new TexturedMesh(vertexList, lineList, UVFaceList, materialList2);
		TexturedMeshProperty* textMeshProp2 = new TexturedMeshProperty(textMesh2);
		obj1->position = { 0,0,0 };
		obj1->addProperty(textMeshProp);
		obj1->setParent(root);
		obj2->position = { -10,0,0 };
		obj2->addProperty(textMeshProp2);
		//obj2->setParent(root);
	}
	Object* obj3 = new Object();
	Object* obj4 = new Object();
	{
		std::vector<Vec3> vertexList = {
			{10.0,10.0,0.0},
			{10.0,-10.0,0.0},
			{-10.0,10.0,0.0},
			{-10.0,-10.0,0.0},
		};
		std::vector<Line> lineList = {};
		std::vector<UVFace> UVFaceList = {
			{0,		0,1,2,		{180,180},{180,0},{0,180}},
			{0,		2,1,3,		{0,180},{180,0},{0,0}},
		};
		std::vector<Material> materialList = {
			{(WCHAR*)L".\\Resource\\Images\\Texture\\square.bmp", 1.0},
		};

		TexturedMesh* textMesh = new TexturedMesh(vertexList, lineList, UVFaceList, materialList);
		TexturedMeshProperty* textMeshProp = new TexturedMeshProperty(textMesh);
		obj3->position = { 0.0,0.0,10.0 };
		obj3->addProperty(textMeshProp);
		obj3->setParent(root);

		obj4->position = { 0.0,-10.0,0.0 };
		Quaternion q = makeQuaternion(-3.1415 / 2, Vec3(1, 0, 0));
		obj4->rotation = Quaternion::multiply(obj4->rotation, q);
		obj4->addProperty(textMeshProp);
		obj4->setParent(root);
	}
	{
		TestLightObject* lightObj = new TestLightObject();
		lightObj->position = { 10.0,0.0,-10.0 };
		LightProperty* lightProp = new LightProperty(LightType::POSITIONAL, 10.0, RGB(255, 100, 100));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	{
		TestLightObject* lightObj = new TestLightObject();
		lightObj->position = { -10.0,0.0,-10.0 };
		LightProperty* lightProp = new LightProperty(LightType::POSITIONAL, 10.0, RGB(100, 255, 100));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	{
		TestLightObject* lightObj = new TestLightObject();
		lightObj->position = { 0.0,10.0,5.0 };
		LightProperty* lightProp = new LightProperty(LightType::POSITIONAL, 10.0, RGB(100, 100, 255));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	/*
	{
		Object* lightObj = new Object();
		LightProperty* lightProp = new LightProperty(LightType::UNIVERSAL, 0.3, RGB(255, 255, 255));
		lightObj->addProperty(lightProp);

		lightObj->setParent(root);
	}
	*/



	camera = new Camera(window, *(window->getResolution()), 100.0, 5.0, 100.0);
	camera->setParent(root);

	camera->position = { 0.0,0.0,-80.0 };
	Quaternion q = makeQuaternion(-0.0, { 1,0,0 });
	camera->rotation = Quaternion::multiply(camera->rotation, q);

	fetchLights();

	return false;
}
