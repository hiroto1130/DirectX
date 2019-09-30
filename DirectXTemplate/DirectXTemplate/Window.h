#ifndef WINDOW_H
#define WINDOW_H

#include"DirectX.h"

class Window
{
public:

// LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
HWND MakeWindow(HWND* ,HINSTANCE, int, int);
HWND MakeFullWindow(HINSTANCE, int, int);

static Window& GetInstance()
{
	Window window;
	return window;
};

};

#endif