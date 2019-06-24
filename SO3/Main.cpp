#include "Window.h"
#include "Platform.h"
#include "Ball.h"
#include "Blocks.h"
#include "Score.h"
#include "Shared.h"
#include <vector>
#include <thread>

using namespace std;

auto main() -> int
{
	//Vector of threads
	vector<thread> threads;
	threads.resize(4);
	//Window initialization
	Window();
	//Shared resources initialization
	Shared();
	//Platfrom initialization
	Platform(Window::Getmaxx(), Window::Getmaxy());
	//Ball initialization
	Ball(Platform::get_position(), Platform::get_floor());
	//Blocks initialization
	Blocks();
	//Score initialization
	Score score;
	//Player input thread
	threads[0] = thread(&Shared::input);
	//Screen refresh thread
	threads[1] = thread(&Window::screen_refresh);
	//Showing score thread
	threads[2] = thread(&Score::draw_score, score);
	//Waiting for ball thread to start(after pressing space)
	mutex spc_mtx;
	unique_lock<mutex> space_pressed_lock(spc_mtx);
	Shared::space_pressed_cv.wait(space_pressed_lock, [] {return Shared::space_pressed.load(); });
	//Ball thread
	threads[3] = thread(&Ball::start_ball, Window::Getmaxx(), Window::Getmaxy());
	//Joining
	for (auto& th : threads)
		th.join();
	Window::close_window();
}