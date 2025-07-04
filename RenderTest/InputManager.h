#pragma once

#include "pch.h"

enum class KEY_STATE {
	NONE,
	DOWN,
	HOLD,
	UP,
};

#define KEY_TYPE UINT
#define KEY (UINT)_KEY
enum class _KEY {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	W,
	A,
	S,
	D,
	R,
	ESC,

	COUNT,
};

struct KeyInfo
{
	KEY_STATE	keyState;
};

class InputManager
{
	SINGLETON(InputManager)

private:
	KeyInfo keyInfo[KEY::COUNT];
	POINT cursorPosition;
	POINT cursorMovement;
	bool cursorFixed;
	POINT cursorFixPosition;

public:
	void init();
	void update();

	KEY_STATE GetKeyState(KEY_TYPE k) { return keyInfo[(int)k].keyState; };
	POINT GetCursorPosition() { return cursorPosition; }
	POINT GetCursorMovement() { return cursorMovement; }

	void FixCursor(POINT);
	void UnfixCursor();
};

