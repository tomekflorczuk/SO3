#include "Ball.h"
short Ball::x;
short Ball::y;
short Ball::prevx;
short Ball::prevy;
short Ball::velocity;
short Ball::direction;
short Ball::deflection_counter;

Ball::Ball(short position, short floor)
{
	x = position;
	y = floor - 1;
	prevx = -1;
	prevy = -1;
	velocity = 80;
	deflection_counter = 0;
	std::uniform_int_distribution<std::mt19937::result_type> const dir_gen(0, 1);
	int const gen_dir = dir_gen(Shared::rng);
	if (gen_dir == 0) direction = 5;
	else direction = 7;
	mvprintw(y, x, "o");
}

void Ball::start_ball(const short maxx, const short maxy)
{
	while(Shared::running.load())
	{
		deflect(maxx, maxy);
		move_ball();
		hit_block();
		//Redrawing the ball
		std::shared_lock<std::shared_mutex> ball_lock(Shared::shrd_curses_mtx);
		mvprintw(y, x, "o");
		mvprintw(prevy, prevx, " ");
		ball_lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(velocity));
	}
}

void Ball::move_ball()
{
	switch (direction)
	{
	//W
	case(0):
	{
		prevy = y;
		prevx = x;
		x--;
		break;
	}
	//SW
	case(1):
	{
		prevx = x;
		x--;
		prevy = y;
		y++;
		break;
	}
	//S
	case(2):
	{
		prevx = x;
		prevy = y;
		y++;
		break;
	}
	//SE
	case(3):
	{
		prevx = x;
		x++;
		prevy = y;
		y++;
		break;
	}
	//E
	case(4):
	{
		prevy = y;
		prevx = x;
		x++;
		break;
	}
	//NE
	case(5):
	{
		prevx = x;
		x++;
		prevy = y;
		y--;
		break;
	}
	//N
	case(6):
	{
		prevx = x;
		prevy = y;
		y--;
		break;
	}
	//NW
	case(7):
	{
		prevx = x;
		x--;
		prevy = y;
		y--;
		break;
	}
	default: break;
	}
}

void Ball::deflect(short const maxx, short const maxy)
{
		if (y == Platform::floor - 1 && (x >= Platform::position - 3 && x <= Platform::position + 3) && prevy != -1)
		{
			deflection_counter++;
			if (x > prevx || x == 0) direction = 5;
			else if (x < prevx || x == maxx - 1) direction = 7;
			else direction = 6;
			velocity = 80;
		}
		else if (y == 0)
		{
			deflection_counter++;
			if (x > prevx || x == 0) direction = 3;
			else if (x < prevx || x == maxx - 1) direction = 1;
			else direction = 2;
		}
		else if (x == maxx - 1)
		{
			deflection_counter++;
			if (y > prevy) direction = 1;
			else if (y < prevy) direction = 7;
			else direction = 0;
		}
		else if (x == 0)
		{
			deflection_counter++;
			if (y > prevy) direction = 3;
			else if (y < prevy) direction = 5;
			else direction = 4;
		}
		else if (y == Platform::floor - 1 && prevy != -1)
		{
			Shared::running.store(false);
			Shared::score_changed.store(true);
			Shared::score_changed_cv.notify_one();
			Shared::platform_freeze.store(false);
			Shared::platform_freeze_cv.notify_one();
		}
	if(deflection_counter == 1)
	{
		Shared::platform_freeze.store(false);
		Shared::platform_freeze_cv.notify_one();
		deflection_counter = 0;
	}
}

void Ball::hit_block()
{
	std::shared_lock<std::shared_mutex> blocks_lock(Shared::shrd_curses_mtx);
	char const sign = mvinch(y, x) & A_CHARTEXT;
	blocks_lock.unlock();
	switch(sign)
	{
	case('X'):
		{
		Shared::score += 10;
		Shared::score_changed.store(true);
		Shared::score_changed_cv.notify_one();
		break;
		}
	case('D'):
		{
		Shared::score += 20;
		Shared::score_changed.store(true);
		Shared::score_changed_cv.notify_one();
		break;
		}
	case('F'):
		{
		Shared::platform_freeze.store(true);
		break;
		}
	case('A'):
		{
		velocity /= 2;
		break;
		}
	default: break;
	}
}

void Ball::redraw_at_platform(short new_x)
{
	std::mutex curses_mtx;

	prevx = x;
	x = new_x;
	std::unique_lock<std::mutex> redraw_lock(curses_mtx);
	mvprintw(y, x, "o");
	mvprintw(y, prevx, " ");
	redraw_lock.unlock();
}
