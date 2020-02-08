#pragma once
#include "std/forward.h"

typedef struct HWND__*	HWND;
typedef struct HINSTANCE__* HINSTANCE;
class WINDOWIMPL
{
public:
	friend class Keyboard;
	friend class Mouse;
	friend class Window;
	friend class Graphics;
	inline static HWND WINDOW;
	static void init(HINSTANCE instance, int x, int y,
		vec2* WINDOW_SIZE,
		vec2* WINDOW_POS,	vec2* MOUSE_POS, vec2 (*MOUSE_CLICK)[3],
		char* KEY_INPUT,	char* MOUSE_INPUT
	);
};