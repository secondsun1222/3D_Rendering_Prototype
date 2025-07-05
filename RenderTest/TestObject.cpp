#include "TestObject.h"
#include "TimeManager.h"
#include "InputManager.h"

TestObject::TestObject()
	:Object()
{
	init();
}


bool TestObject::init()
{
	return true;
}

bool TestObject::update()
{

	double deltaTime = INSTANCE(TimeManager)->DeltaTime;
	POINT cursorMovement = INSTANCE(InputManager)->GetCursorMovement();

	if (INSTANCE(InputManager)->GetKeyState(KEY::MOUSER) == KEY_STATE::NONE)
	{

		Quaternion cursorRotationX = makeQuaternion(cursorMovement.x / 10.0 * deltaTime, Vec3{ 0.0,1.0,0.0 });
		rotation = Quaternion::multiply(cursorRotationX, rotation);

		Vec3 dir = Quaternion::rotate(rotation, { 1,0,0 });
		Quaternion cursorRotationY = makeQuaternion(-cursorMovement.y / 10.0 * deltaTime, Vec3{ 1.0,0.0,0.0 });
		rotation = Quaternion::multiply(cursorRotationY, rotation);
	}
	return true;
}
