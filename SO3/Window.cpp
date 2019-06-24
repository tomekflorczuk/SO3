#include "Window.h"

short Window::maxx;
short Window::maxy;

Window::Window()
{
	initscr();
	curs_set(0);
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);
}

void Window::screen_refresh()
{
	while(Shared::running.load())
	{
		std::shared_lock<std::shared_mutex> refresh_lock(Shared::shrd_curses_mtx);
		refresh();
		refresh_lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void Window::close_window()
{
	endwin();
}
