#pragma once
#include "Shared.h"

class Window
{
	static short maxx, maxy;
public:
	Window();
	~Window() = default;
	static short Getmaxx() { return maxx; }
	static short Getmaxy() { return maxy; }
	static void screen_refresh();
	static void close_window();
};