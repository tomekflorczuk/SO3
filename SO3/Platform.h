#pragma once
#include "Shared.h"

class Platform
{
public:
	static short floor, position;
	Platform(short maxx, short maxy);
	~Platform() = default;
	static void move_left();
	static void move_right();
	static short get_position() {return position; }
	static short get_floor() { return floor; }
};