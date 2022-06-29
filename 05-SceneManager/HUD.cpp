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
	card= CAnimations::GetInstance();
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
		for (size_t i = 0; i < mario->GetCards().size(); i++)
		{
			int ani = 0;
			if (mario->GetCards().at(i)== ID_SPRITE_STAR)
			{
				ani = ID_ANI_STAR;
			}
			else if (mario->GetCards().at(i) == ID_SPRITE_FLOWER) {
				ani = ID_ANI_FLOWER;
			}
			else if (mario->GetCards().at(i) == ID_SPRITE_MUSHROOM) {
				ani = ID_ANI_MUSHROOM;
			}
			switch (i)
			{
			case 0:
				
				card->Get(ani)->Render(x + card_1_margin_x, y + card_margin_y);
				break;
			case 1:
				card->Get(ani)->Render(x + card_2_margin_x, y + card_margin_y);
				break;
			case 2:
				card->Get(ani)->Render(x + card_3_margin_x, y + card_margin_y);
				break;
			}
		}
	}
	
	speedBar->Render(vx,mario->GetIsPowerUp());
}