#include "TestLightObject.h"

TestLightObject::TestLightObject()
	:Object()
{
	lightProp = nullptr;
	init();
}

bool TestLightObject::init()
{
	return true;
}

bool TestLightObject::update()
{
	if (!lightProp) {
		lightProp = (LightProperty*)findProperty(LIGHT);
	}

	if (lightProp) {
		lightProp->brightness += INSTANCE(InputManager)->GetScrollMovement();
		lightProp->brightness = min(max(lightProp->brightness, 0.0), 100.0);
	}

	double speed = 10.0;

	if (INSTANCE(InputManager)->GetKeyState(KEY::A) == KEY_STATE::HOLD) position.x += speed * INSTANCE(TimeManager)->DeltaTime;
	if (INSTANCE(InputManager)->GetKeyState(KEY::D) == KEY_STATE::HOLD) position.x -= speed * INSTANCE(TimeManager)->DeltaTime;
	if (INSTANCE(InputManager)->GetKeyState(KEY::W) == KEY_STATE::HOLD) position.z += speed * INSTANCE(TimeManager)->DeltaTime;
	if (INSTANCE(InputManager)->GetKeyState(KEY::S) == KEY_STATE::HOLD) position.z -= speed * INSTANCE(TimeManager)->DeltaTime;
	position.x = max(min(position.x, 10.0), -10.0);
	position.z = max(min(position.z, 10.0), -10.0);
	return true;
}
