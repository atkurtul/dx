#define WIN32_LEAN_AND_MEAN
#include "Detail.h"
#include <Windows.h>
#include "std/vec.h"


inline static vec2* WINDOW_SIZE			= nullptr;
inline static vec2* WINDOW_POS			= nullptr;
inline static vec2* MOUSE_POS			= nullptr;
inline static vec2	(*MOUSE_CLICK)[3]	= nullptr;
inline static char* KEY_INPUT			= nullptr;
inline static char* MOUSE_INPUT			= nullptr;

#define MPOS(lparam) vec2((short&)lparam, WINDOW_SIZE->y - (lparam >> 16))

LRESULT __stdcall WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN: 
	{
		if (wparam == VK_ESCAPE)	PostQuitMessage(69);
		KEY_INPUT[wparam] = 1 + bool(KEY_INPUT[wparam]);
		break;
	}
	case WM_KEYUP: 			KEY_INPUT[wparam] = 0;	break;
	//case WM_CHAR: 			charbuffer.push(wparam);	break;
	case WM_LBUTTONDOWN: 	if (!MOUSE_INPUT[0]) MOUSE_CLICK[0][0] = MPOS(lparam);
							MOUSE_INPUT[0] = 1 + bool(MOUSE_INPUT[0]);	break;
	case WM_LBUTTONUP: 		MOUSE_INPUT[0] = 0;	break;
	case WM_RBUTTONDOWN: 	if (!MOUSE_INPUT[1]) MOUSE_CLICK[1][0] = MPOS(lparam); 
							MOUSE_INPUT[1] = 1 + bool(MOUSE_INPUT[1]); break;
	case WM_RBUTTONUP: 		MOUSE_INPUT[1] = 0; break;
	case WM_MBUTTONDOWN: 	if (!MOUSE_INPUT[2]) MOUSE_CLICK[2][0] = MPOS(lparam); 
							MOUSE_INPUT[2] = 1 + bool(MOUSE_INPUT[2]); break;
	case WM_MBUTTONUP: 		MOUSE_INPUT[2] = 0;	break;
	case WM_MOUSEMOVE:		*MOUSE_POS = MPOS(lparam); break;
	case WM_MOVE:			*WINDOW_POS = vec2((short&)lparam, lparam >> 16); break;
	case WM_INPUT:
	case WM_CLOSE:			PostQuitMessage(69); break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WINDOWIMPL::init(HINSTANCE instance, int x, int y,
	vec2* window_size, vec2* window_pos, vec2* mouse_pos, vec2 (*mouse_click)[3], char* key_input, char* mouse_input)
{
	 WINDOW_SIZE		= window_size;
	 WINDOW_POS			= window_pos;
	 MOUSE_POS			= mouse_pos;
	 MOUSE_CLICK		= mouse_click;
	 KEY_INPUT			= key_input;
	 MOUSE_INPUT		= mouse_input;


	 auto cname = L"DirectX";
	 WNDCLASSEXW wc = {};
	 wc.cbSize = sizeof(WNDCLASSEX);
	 wc.style = CS_OWNDC;
	 wc.lpfnWndProc = WindowProc;
	 wc.cbClsExtra = 0;
	 wc.cbWndExtra = 0;
	 wc.hInstance = (HINSTANCE)instance;
	 wc.hIcon = 0;
	 wc.hCursor = 0;
	 wc.hbrBackground = 0;
	 wc.lpszMenuName = 0;
	 wc.lpszClassName = cname;
	 wc.hIcon = 0;
	 RegisterClassExW(&wc);
	 WINDOW = CreateWindowExW(
		 0, cname, L"DirectX3D",
		 WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		 0, 0, x + 11, y + 34,
		 0, 0, instance, 0
	 );
	 ShowWindow(WINDOW, SW_SHOW);
	 UpdateWindow(WINDOW);

}
