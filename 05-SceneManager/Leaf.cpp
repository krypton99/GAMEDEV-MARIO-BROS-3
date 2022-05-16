#include "Leaf.h"
#include "GameObject.h"
#include "Mario.h"
#include "AssetIDs.h"

#define BRICK_BOUNDING_X 5.0f
CLeaf::CLeaf(float x, float y, float leafType)
{
	this->x = x;
	this->y = y;
	ItemType = leafType;
	vx = -ITEM_SPEED_Y;
	this->ax = 0;
	this->ay = ITEM_GRAVITY;
	start_x = x;
}
void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CLeaf*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	
}

void CLeaf::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int ani;
	if (vx > 0) { ani = ID_ITEM_ANI_LEAF_RIGHT; }
	else  ani = ID_ITEM_ANI_LEAF_LEFT;

	animations->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Item::Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	if (!isFalling)
		vy += (ITEM_GRAVITY * dt);
	else
		vy = LEAF_SPEED_FALL;


	if (!isFalling)
	{
		isFalling = true;
		vx = -LEAF_SPEED_X;
	}

	if (isFalling)
	{
		if (vx < 0 && x < start_x - BRICK_BBOX)
		{
			vx = -vx;
			x = start_x - BRICK_BBOX;
		}
		if (vx > 0 && x > start_x + BRICK_BBOX)
		{
			vx = -vx;
			x = start_x + BRICK_BBOX;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
