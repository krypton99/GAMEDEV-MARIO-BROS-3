#include "SpeedBar.h"
#include "Mario.h"

void CSpeedBar::Render(float vx)
{
	CAnimations* animations = CAnimations::GetInstance();
	float speed_level = (MARIO_RUNNING_SPEED - MARIO_WALKING_SPEED) / 7;
	int aniId;
	if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 6)
		aniId = ID_ANI_SPEED_7;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 5)
		aniId = ID_ANI_SPEED_6;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 4)
		aniId = ID_ANI_SPEED_5;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 3)
		aniId = ID_ANI_SPEED_4;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 2)
		aniId = ID_ANI_SPEED_3;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 1)
		aniId = ID_ANI_SPEED_2;
	else if (abs(vx) > MARIO_WALKING_SPEED + speed_level * 0)
		aniId = ID_ANI_SPEED_1;
	else aniId = ID_ANI_SPEED_0;
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

