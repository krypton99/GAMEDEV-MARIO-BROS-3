#include "CoinEffect.h"
#include "Utils.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Coin.h"


CCoinEffect::CCoinEffect(float x, float y)
{
	this->type = OBJECT_TYPE_COIN;
	this->x = x;
	this->y = y-BRICK_BBOX;

	GetPosition(start_x, start_y);
	vy = -COIN_SPEED_Y;
	isBound = false;
}

void CCoinEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CCoinEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += vy*dt;
	if (state != STATE_ERASE)
		vy += MARIO_GRAVITY * dt;

	else
		vy = 0;

	if (vy >= 0)
		isBound = true;

	if (isBound && y > start_y)
		state = STATE_ERASE;
}

void CCoinEffect::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);
	//RenderBoundingBox();
}