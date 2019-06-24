#pragma once
#include "Shared.h"

class Ball
{
	static short x, y, prevx, prevy, velocity, direction, deflection_counter;
public:
	Ball(short position, short floor);
	~Ball() = default;
	static void start_ball(short maxx, short maxy);
	static void move_ball();
	static void deflect(short maxx, short maxy);
	static void hit_block();
	static void redraw_at_platform(short new_x);
};