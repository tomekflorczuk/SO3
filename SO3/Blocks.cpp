#include "Blocks.h"

Blocks::Blocks()
{
	start_y = 10;
	screen_center = Window::Getmaxx() / 2;
	int it = 10;
	for (int y = start_y; y < 15; y++)
	{
		for (int x = screen_center - it; x <= screen_center + it; x++)
		{
			char const sign = pick_random_block_sign();
			mvprintw(y, x, "%c", sign);
		}
		it--;
	}
}

char Blocks::pick_random_block_sign()
{
	std::uniform_int_distribution<std::mt19937::result_type> const block_gen(1, 10);

	int const block_probabiility = block_gen(Shared::rng);

	if (block_probabiility <= 7) return 'X';
	if (block_probabiility == 8) return 'D';
	if (block_probabiility == 9) return 'F';
	if (block_probabiility == 10) return 'A';
}
