#include "InputManager.h"

static int VKarr[KEY::COUNT] = {
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,
	'W',		//LEFT,
	'A',		//RIGHT,
	'S',		//UP,
	'D',		//DOWN,
	'R',		//R
	VK_ESCAPE,	//ESC
	VK_CONTROL,	//CTRL
	VK_RBUTTON,

	//
	//COUNT,
};

InputManager::InputManager()
	: keyInfo{}
{

}

InputManager::~InputManager()
{

}

void InputManager::init()
{
	for (int i = 0; i < KEY::COUNT; i++)
	{
		keyInfo[i] = { KEY_STATE::NONE };
	}

	GetCursorPos(&cursorPosition);
	cursorMovement = { 0,0 };
	cursorFixed = false;
	cursorFixPosition = { 0,0 };
	scrollMovement = 0;
	didScroll = false;
}

void InputManager::update()
{
	HWND hWndFocused = GetFocus();

	//check keyboard inputs
	for (int i = 0; i < KEY::COUNT; i++)
	{
		// true when focused and key pressed
		bool keyDown = hWndFocused != NULL && GetAsyncKeyState(VKarr[i]) & 0x8000;

		if (keyDown) {
			switch (keyInfo[i].keyState)
			{
			case KEY_STATE::NONE:
			case KEY_STATE::UP:
				keyInfo[i].keyState = KEY_STATE::DOWN;
				break;

			case KEY_STATE::DOWN:
			case KEY_STATE::HOLD:
				keyInfo[i].keyState = KEY_STATE::HOLD;
				break;

			default:
				break;
			}
		}
		else {
			switch (keyInfo[i].keyState)
			{
			case KEY_STATE::NONE:
			case KEY_STATE::UP:
				keyInfo[i].keyState = KEY_STATE::NONE;
				break;

			case KEY_STATE::DOWN:
			case KEY_STATE::HOLD:
				keyInfo[i].keyState = KEY_STATE::UP;
				break;

			default:
				break;
			}
		}
	}

	// apply cursor position
	POINT newCursorPosition;
	GetCursorPos(&newCursorPosition);
	cursorMovement = { newCursorPosition.x - cursorPosition.x, newCursorPosition.y - cursorPosition.y };
	cursorPosition = newCursorPosition;

	// fix cursor?
	if (cursorFixed) {
		SetCursorPos(cursorFixPosition.x, cursorFixPosition.y);
		cursorPosition = cursorFixPosition;
	}


	// scroll controls
	if (didScroll) {
		scrollMovement = 0;
		didScroll = false;
	}
	if (scrollMovement != 0)didScroll = true;
}

void InputManager::FixCursor(POINT _cursorFixPosition)
{
	cursorFixed = true;
	cursorFixPosition = _cursorFixPosition;
}

void InputManager::UnfixCursor()
{
	cursorFixed = false;
}
