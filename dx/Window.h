#pragma once
#include "Clock.h"
#include "Input.h"
#include "std/string.h"
#include "Detail.h"
class Keyboard
{
	friend class Window;
	friend class Input;
	inline static char Keys[256];
};

class Mouse
{
	friend class Window;
	friend class Input;
	inline static char Buttons[3];
	inline static bool Shown;
	static void Update();
public:
	inline static vec2 Click[3];
	inline static vec2 Pos;
	inline static vec2 Delta;
	inline static vec2 Normalized;
	static bool CheckPos(const vec4& rect);
	static bool CheckBtn(const vec4& rect, uint btn);

	static void Hide();
	static void Show();
};

class Window
{
	friend class Graphics;
	friend class Keyboard;
	friend class Mouse;
	friend class Cam;
public:
	inline static vec2 InvSize;
	inline static vec2 Size;
	inline static vec2 Pos;
	static void Init(HINSTANCE instance, float x, float y);
	static int  PollEvents();
	static void Flush(void* instance);
	static void SetTitle(const String& title);
};
