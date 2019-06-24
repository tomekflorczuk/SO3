#pragma once
#include "Ball.h"
#include "Platform.h"
#include "Window.h"
#include <curses.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <random>

class Shared
{
	static std::random_device rdev;
public:
	Shared();
	~Shared() = default;
	static std::atomic<bool> running, space_pressed, score_changed, platform_freeze;
	static std::condition_variable space_pressed_cv, score_changed_cv, platform_freeze_cv;
	static std::shared_mutex shrd_curses_mtx;
	static std::atomic<short> score;
	static std::mt19937 rng;

	static void input();
};

