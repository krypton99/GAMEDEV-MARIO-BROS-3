#pragma once
#include "Game.h"
#include "Mario.h"
#define ID_ANI_BOARD 9901
#define ID_ANI_BACKGROUND 9999

#define BOARD_WIDTH	232
#define BOARD_HEIGHT 28

#define BOARD_LEFT_MARGIN 8


class HUD
{
	CAnimations* board = nullptr;
	CAnimations* background = nullptr;
public:
	HUD() {};

	void Render(float x, float y, CMario* mario);
};
typedef HUD* LPHUD;