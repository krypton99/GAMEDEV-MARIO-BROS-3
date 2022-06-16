#include "HUD.h"
#include "Camera.h"

void HUD::Render(float x, float y, CMario* mario) {
	float vx, vy;
	mario->GetSpeed(vx, vy);
	speedBar = new CSpeedBar(x + 92, y + SCREEN_HEIGHT / 2 + 32);
	board = CAnimations::GetInstance();
	background = CAnimations::GetInstance();
	background->Get(ID_ANI_BACKGROUND)->Render(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2 + 64);
	board->Get(ID_ANI_BOARD)->Render(x + BOARD_WIDTH / 2 + 8, y + SCREEN_HEIGHT / 2 + 36);
	speedBar->Render(vx);
}