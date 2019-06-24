#include "Score.h"

void Score::draw_score() const
{
	std::shared_lock<std::shared_mutex> score_lock(Shared::shrd_curses_mtx);
	mvprintw(y, x, "%i", Shared::score.load());
	score_lock.unlock();
	while(Shared::running.load())
	{
		std::mutex score_chngd_mtx;
		std::unique_lock<std::mutex> score_chngd_lock(score_chngd_mtx);
		Shared::score_changed_cv.wait(score_chngd_lock, [] {return Shared::score_changed.load(); });
		Shared::score_changed.store(false);
		score_lock.lock();
		mvprintw(y, x, "%i", Shared::score.load());
		score_lock.unlock();
	}
}