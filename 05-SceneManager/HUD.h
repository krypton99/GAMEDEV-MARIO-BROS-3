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

#define score_margin_y		160
#define score_margin_x		64
#define number_offset	8
#define coin_margin_y	152
#define coin_margin_x	152

#define time_margin_y	160
#define time_margin_x	136

#define card_1_margin_x		180
#define card_2_margin_x		205
#define card_3_margin_x		228
#define card_margin_y		155
#define ID_ANI_STAR 9040	
#define ID_ANI_MUSHROOM 9041
#define ID_ANI_FLOWER 9042
#define ID_SPRITE_STAR 90040
#define ID_SPRITE_MUSHROOM 90041
#define ID_SPRITE_FLOWER 90042
class HUD
{
	CSpeedBar* speedBar = nullptr;
	CAnimations* board = nullptr;
	CAnimations* background = nullptr;
	CAnimations* card;
	Text text;
public:
	HUD() {};

	void Render(float x, float y, CMario* mario, int remainingTime);
};
typedef HUD* LPHUD;