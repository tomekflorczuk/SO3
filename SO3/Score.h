#pragma once
#include "Shared.h"

class Score
{
	short const x = 0;
	short const y = Window::Getmaxy() - 1;
public:
	~Score() = default;
	void draw_score() const;
};