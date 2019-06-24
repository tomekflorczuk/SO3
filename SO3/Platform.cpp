#include "Platform.h"

short Platform::position;
short Platform::floor;


Platform::Platform(short maxx, short maxy)
{
	position = maxx / 2;
	floor = maxy - 1;
	for (int i = position - 3; i <= position + 3; i++)
		mvprintw(floor, i, "T");
}

void Platform::move_left()
{
	if (position - 3 != 0)
	{
		std::shared_lock<std::shared_mutex> curses_lock(Shared::shrd_curses_mtx);
		mvprintw(floor, position + 3, " ");
		position--;
		mvprintw(floor, position - 3, "T");
		curses_lock.unlock();
		if (!Shared::space_pressed)
		{
			Ball::redraw_at_platform(position);
		}
	}
}

void Platform::move_right()
{
	if (position + 3 != Window::Getmaxx() - 1)
	{
		std::shared_lock<std::shared_mutex> curses_lock(Shared::shrd_curses_mtx);
		mvprintw(floor, position - 3, " ");
		position++;
		mvprintw(floor, position + 3, "T");
		curses_lock.unlock();
		if (!Shared::space_pressed)
		{
			Ball::redraw_at_platform(position);
		}
	}
}