#include "Shared.h"

std::atomic<bool>Shared::running;
std::atomic<bool> Shared::space_pressed;
std::atomic<bool> Shared::score_changed;
std::atomic<bool> Shared::platform_freeze;
std::condition_variable Shared::space_pressed_cv;
std::condition_variable Shared::score_changed_cv;
std::condition_variable Shared::platform_freeze_cv;
std::shared_mutex Shared::shrd_curses_mtx;
std::atomic<short> Shared::score;
std::random_device Shared::rdev;
std::mt19937 Shared::rng(rdev());

Shared::Shared()
{
	running.store(true);
	space_pressed.store(false);
	score_changed.store(false);
	platform_freeze.store(false);
	score.store(0);
}

void Shared::input()
{
	int key;
	std::mutex input_mtx, platform_freeze_mtx;

	while(running.load())
	{	
		if (platform_freeze.load())
		{
			std::unique_lock<std::mutex> platform_freeze_lock(platform_freeze_mtx);
			platform_freeze_cv.wait(platform_freeze_lock, [] {return !platform_freeze.load(); });
			platform_freeze_lock.unlock();
		}
		std::unique_lock<std::mutex> input_lock(input_mtx);
		key = getch();
		input_lock.unlock();
		switch (key)
		{
		case('x'):
		{
			running.store(false);
			space_pressed.store(true);
			space_pressed_cv.notify_one();
			score_changed.store(true);
			score_changed_cv.notify_one();
			platform_freeze.store(false);
			platform_freeze_cv.notify_one();
			break;
		}
		case(KEY_LEFT):
		{
			Platform::move_left();
			break;
		}
		case(KEY_RIGHT):
			{
			Platform::move_right();
			break;
			}
		case(' '):
			{
			if (!space_pressed.load())
			{
				space_pressed.store(true);
				space_pressed_cv.notify_one();
			}
			break;
			}
		default: break;
		}
	}
}