#pragma once
#include "Shared.h"

class Blocks
{
	short start_y, screen_center;
public:
	Blocks();
	~Blocks() = default;
	char pick_random_block_sign();
};