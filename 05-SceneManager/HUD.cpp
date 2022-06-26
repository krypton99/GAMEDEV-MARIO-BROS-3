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

void HUD::Render(float x, float y, CMario* mario) {
	float vx, vy;
	mario->GetSpeed(vx, vy);
	board = CAnimations::GetInstance();
	background = CAnimations::GetInstance();

	
	if (mario->GetStage() == WORLD_MAP_SCENE) {
		speedBar = new CSpeedBar(x + 92, y + SCREEN_HEIGHT / 2 + SPEEDBAR__OFFSET+WORLDMAP_OFFSET);
		background->Get(ID_ANI_BACKGROUND)->Render(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2 + BACKGROUND_OFFSET + WORLDMAP_OFFSET);
		board->Get(ID_ANI_BOARD)->Render(x + BOARD_WIDTH / 2 + 8, y + SCREEN_HEIGHT / 2 + BOARD_OFFSET + WORLDMAP_OFFSET);
	}
	else {
		speedBar = new CSpeedBar(x + 92, y + SCREEN_HEIGHT / 2 + SPEEDBAR__OFFSET);
		background->Get(ID_ANI_BACKGROUND)->Render(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2 + BACKGROUND_OFFSET);
		board->Get(ID_ANI_BOARD)->Render(x + BOARD_WIDTH / 2 + 8, y + SCREEN_HEIGHT / 2 + BOARD_OFFSET);
		text.Render(IntToChar(mario->getScore(),7),  x + time_margin_x, y + money_margin_y );
	}
	
	speedBar->Render(vx,mario->GetIsPowerUp());
}