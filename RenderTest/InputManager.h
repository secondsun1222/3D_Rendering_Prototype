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
	CTRL,
	MOUSER,

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

	int scrollMovement;
	bool didScroll;

public:
	void init();
	void update();

	KEY_STATE GetKeyState(KEY_TYPE k) { return keyInfo[(int)k].keyState; };
	POINT GetCursorPosition() { return cursorPosition; }
	POINT GetCursorMovement() { return cursorMovement; }
	void SetScrollMovement(int _scrollMovement) { didScroll = false; scrollMovement = _scrollMovement; }
	int GetScrollMovement() { return scrollMovement; }

	void FixCursor(POINT);
	void UnfixCursor();
};

