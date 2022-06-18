#include "BrickBrokenPieces.h"


CBrickBrokenPieces::CBrickBrokenPieces(float x, float y, int nx, float v)
{
	this->x = x;
	this->y = y;
	this->vx = BROKEN_SPEED_X * nx;
	this->vy = -BROKEN_SPEED_Y * v;
}
void CBrickBrokenPieces::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK_BROKEN)->Render(x, y);

	//RenderBoundingBox();
}
void CBrickBrokenPieces::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.0008f * dt;
	x += vx * dt;
	y += vy * dt;
	/*if (this->isOutOfCam())
		SetState(STATE_DESTROYED);*/
}