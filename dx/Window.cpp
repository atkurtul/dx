#define WIN32_LEAN_AND_MEAN
#include "Detail.h"
#include <Windows.h>
#include "Window.h"


void Window::Init(HINSTANCE instance, float x, float y)
{
	Size = { x, y };
	InvSize = 1.f / Size;
	WINDOWIMPL::init(instance, x, y,
		&Size, &Pos, &Mouse::Pos, &Mouse::Click, Keyboard::Keys, Mouse::Buttons
		);
}

void Mouse::Update()
{
	if (Shown)
	{
		//static vec2 prev = Pos;
		//Delta = Pos - prev;
		//prev = Pos;
		Delta = 0;
	}
	else
	{
		Delta = Pos - Window::Size * 0.5f;
		SetCursorPos(int(Window::Size.x * 0.5f + Window::Pos.x), int(Window::Size.y * 0.5f + Window::Pos.y));
	}
	Normalized = 2.f * ( Pos / Window::Size) - 1.f;
}

bool Mouse::CheckPos(const vec4& rect)
{
	return (Pos.x > rect.x && 
			Pos.y > rect.y &&
			Pos.x < rect.x + rect.z &&
			Pos.y < rect.y + rect.w);
}

bool Mouse::CheckBtn(const vec4& rect, uint btn)
{
	return (Click[btn].x > rect.x && 
			Click[btn].y > rect.y &&
			Click[btn].x < rect.x + rect.z &&
			Click[btn].y < rect.y + rect.w);
}

void Mouse::Hide()
{
	if (!Shown) return;
	Delta = 0;
	Shown = 0;
	Pos = Window::Size * 0.5f;
	SetCursorPos(int(Window::Size.x * 0.5f + Window::Pos.x), int(Window::Size.y * 0.5f + Window::Pos.y));
	while (ShowCursor(0) >= 0);
}

void Mouse::Show()
{
	Delta = 0;
	Shown = 1;
	while (ShowCursor(1) < 0);
}


int Window::PollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return 0;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	Clock::Update();
	Mouse::Update();
	return 1;
}

void Window::Flush(void* instance)
{

	UnregisterClassW(L"DirectX", (HINSTANCE)instance);
}

void Window::SetTitle(const String& title)
{
	SetWindowTextA((HWND)WINDOWIMPL::WINDOW, title.c_str());
}

char Input::Key(unsigned char keycode)
{
	return Keyboard::Keys[keycode];
}

char Input::Button(unsigned char keycode)
{
	return Mouse::Buttons[keycode];
}
