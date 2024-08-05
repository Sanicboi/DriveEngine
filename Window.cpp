#include "Window.h"


Window::Window(HINSTANCE hinstance, LPCWSTR name, int w, int h)
{
	const LPCWSTR CLASS_NAME = L"Drive Window";
	WNDCLASS wc = {};
	wc.hInstance = hinstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		w, h,
		NULL, NULL,
		hinstance,
		NULL
	);
}

bool Window::Ok() const
{
	return hwnd != NULL;
}

HWND Window::GetHWND() const
{
	return hwnd;
}

void Window::Show(int param) const
{
	if (Ok()) {
		ShowWindow(hwnd, param);
	}
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
