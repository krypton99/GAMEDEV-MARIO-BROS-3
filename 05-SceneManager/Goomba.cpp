#include "Goomba.h"
#include "AssetIDs.h"

CGoomba::CGoomba(float x, float y, float goomba_type):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	this->goomba_type = goomba_type;
	//this->isBlocking = false;
	type = OBJECT_TYPE_GOOMBA;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	timeStartJump->Start();
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		if (type != GOOMBA_TYPE_RED_WING) {
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
		else {
			left = x - GOOMBA_BBOX_WING_WIDTH / 2;
			top = y - GOOMBA_BBOX_WING_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WING_WIDTH;
			bottom = top + GOOMBA_BBOX_WING_HEIGHT;
		}
	}
}


void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
		if (e->ny < 0) // va cham ground
		{
			isOnGround = true;
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		state = STATE_ERASE;
		return;
	}
	if (goomba_type == GOOMBA_TYPE_RED_WING) {
		if (timeStartJump->IsTimeUp() && timeStartJump->GetStartTime()) { // bd tinh time nhay
			timeStartJump->Stop();
			SetState(GOOMBA_STATE_JUMP);
		}
		if (state == GOOMBA_STATE_JUMP && isOnGround) {
			SetState(GOOMBA_STATE_FLY);
		}
	}
	//CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	if (goomba_type == GOOMBA_TYPE_RED_WING) {
		int aniId = ID_ANI_RED_GOOMBA_FLY;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else if (goomba_type == GOOMBA_TYPE_RED) {
		int aniId = ID_ANI_RED_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE)
		{
			aniId = ID_ANI_RED_GOOMBA_DIE;
		}
		else if (state == GOOMBA_STATE_DIE_BY_OBJECT) {
			aniId = ID_ANI_RED_GOOMBA_DIE_BY_OBJECT;
		}
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else {
		int aniId = ID_ANI_BROWN_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE)
		{
			aniId = ID_ANI_BROWN_GOOMBA_DIE;
		}
		else if (state == GOOMBA_STATE_DIE_BY_OBJECT) {
			aniId = ID_ANI_BROWN_GOOMBA_DIE_BY_OBJECT;
		}

		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	if (/*this->state == GOOMBA_STATE_DIE|| this->state==GOOMBA_STATE_DIE_BY_OBJECT ||*/ this->state == STATE_ERASE) return;
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE_BY_OBJECT:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case GOOMBA_STATE_FLY:
		timeStartJump->Start();
		isOnGround = false;
		vy = -GOOMBA_FLY_SPEED;
		vx = vx;

	case GOOMBA_STATE_JUMP:
		isOnGround = false;
		vy = -GOOMBA_JUMP_SPEED;

	}
}
