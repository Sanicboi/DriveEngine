#pragma once
#include "Main.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
private:
	HWND hwnd;

public:
	Window(HINSTANCE hinstance, LPCWSTR name, int w, int h);
	bool Ok() const;
	HWND GetHWND() const;
	void Show(int param) const;
};