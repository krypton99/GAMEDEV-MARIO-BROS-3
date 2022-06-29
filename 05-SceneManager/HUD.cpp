#include "HUD.h"
#include "Camera.h"
#include <stdlib.h>

char* IntToChar(int value, int length)
{
	char* c = new char[length + 1];
	c[length] = '\0';
	for (int i = length - 1; i >= 0; i--)
	{
		c[i] = value % 10 + 48;
		value = value / 10;
	}
	return c;
}

void HUD::Render(float x, float y, CMario* mario, int remainingTime) {
	float vx, vy;
	mario->GetSpeed(vx, vy);
	board = CAnimations::GetInstance();
	background = CAnimations::GetInstance();
	char buffer[3];
	
	if (mario->GetStage() == WORLD_MAP_SCENE) {
		speedBar = new CSpeedBar(x + 92, y + SCREEN_HEIGHT / 2 + SPEEDBAR__OFFSET+WORLDMAP_OFFSET);
		background->Get(ID_ANI_BACKGROUND)->Render(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2 + BACKGROUND_OFFSET + WORLDMAP_OFFSET);
		board->Get(ID_ANI_BOARD)->Render(x + BOARD_WIDTH / 2 + 8, y + SCREEN_HEIGHT / 2 + BOARD_OFFSET + WORLDMAP_OFFSET);
	}
	else {
		speedBar = new CSpeedBar(x + 92, y + SCREEN_HEIGHT / 2 + SPEEDBAR__OFFSET);
		background->Get(ID_ANI_BACKGROUND)->Render(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2 + BACKGROUND_OFFSET);
		board->Get(ID_ANI_BOARD)->Render(x + BOARD_WIDTH / 2 + 8, y + SCREEN_HEIGHT / 2 + BOARD_OFFSET);
		text.Render(IntToChar(mario->getScore(),7),  x + score_margin_x, y + score_margin_y);
		_itoa_s(mario->getCoin(), buffer,3, 10);
		if ((mario->getCoin() / 10) > 0) {
			text.Render(buffer, x + coin_margin_x- number_offset, y + coin_margin_y);
		} else
		text.Render(buffer, x + coin_margin_x, y + coin_margin_y);
		text.Render(IntToChar(remainingTime, 3), x + time_margin_x, y + time_margin_y);
	}
	
	speedBar->Render(vx,mario->GetIsPowerUp());
}