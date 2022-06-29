#pragma once
#include "Game.h"
#include "Mario.h"
#include "SpeedBar.h"
#include "Text.h"
#define ID_ANI_BOARD 9901
#define ID_ANI_BACKGROUND 9999

#define BOARD_WIDTH	232
#define BOARD_HEIGHT 28

#define BOARD_LEFT_MARGIN 8

#define BACKGROUND_OFFSET 64
#define BOARD_OFFSET 36
#define SPEEDBAR__OFFSET 32

#define WORLDMAP_OFFSET 28

#define money_margin_y		160

#define time_margin_x		64
#define number_offset	8
#define coin_margin_y	152
#define coin_margin_x	152
class HUD
{
	CSpeedBar* speedBar = nullptr;
	CAnimations* board = nullptr;
	CAnimations* background = nullptr;
	Text text;
public:
	HUD() {};

	void Render(float x, float y, CMario* mario);
};
typedef HUD* LPHUD;