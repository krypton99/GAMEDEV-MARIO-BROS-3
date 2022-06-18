#include "Pswitch.h"
#include "GameObject.h"
#include "Mario.h"
#include "Grid.h"
#include "Brick.h"
#include "Coin.h"
#define BRICK_BOUNDING_X 5.0f
CPswitch::CPswitch(float x, float y)
{
	this->x = x;
	this->y = y;
	state = SWITCH_STATE_ON;
	this->type = OBJECT_TYPE_PSWITCH;
}


void CPswitch::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int ani = 0;
	if (state == SWITCH_STATE_OFF) {
		ani = ID_ITEM_ANI_PSWITCH_OFF;
	}
	else ani = ID_ITEM_ANI_PSWITCH_ON;

	animations->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CPswitch::Update(DWORD dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* items) {
	CGameObject::Update(dt, objects);

	
	CCollision::GetInstance()->Process(this, dt, objects);
}
void CPswitch::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case SWITCH_STATE_OFF:
		y = y + 10;
		break;
	case SWITCH_STATE_ON:
		break;
	default:
		break;
	}
}
void CPswitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == SWITCH_STATE_OFF)
		l = t = b = r = 0;
	else
	{
		l = x - BRICK_BBOX / 2;
		t = y - BRICK_BBOX / 2;
		r = l + BRICK_BBOX;
		b = t + BRICK_BBOX;
	}
}