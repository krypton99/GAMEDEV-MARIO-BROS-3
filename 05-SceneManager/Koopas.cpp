#include "Koopas.h"
#include "AssetIDs.h"
#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"
#include "Brick.h"
#include "PlayScene.h"

CKoopas::CKoopas(float x, float y, float type) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = TROOPA_GRAVITY;
	this->koopa_type = type;
	this->type = OBJECT_TYPE_KOOPAS;
	isGhostFollow = true;
	type = OBJECT_TYPE_KOOPAS;
	die_start = -1;
	
	SetState(TROOPA_STATE_WALKING);
		
	start_vx = vx;

	/*ghost = new CGhost(x+16, y);*/
	//ghost->Render();
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE || state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT)
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	DebugOut(L"no collision");
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	/*}*/

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) // va cham ground
		{
			
		}
	}
	else if (e->nx != 0)
	{

		vx = -vx;
	}
	
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;
	vx += ax * dt;
	
	ay = TROOPA_GRAVITY;
	
	if (state == TROOPA_STATE_DIE) {
		if (timeReborn->IsTimeUp() && timeReborn->GetStartTime()) { // bd tinh time hoi sinh
			timeReborn->Stop();
			}

		}
	if (koopa_type == KOOPAS_TYPE_RED) {
		if (ghost_koopas->isOnGround == false && (state != TROOPA_STATE_DIE && state != TROOPA_STATE_ROLL_LEFT && state != TROOPA_STATE_ROLL_RIGHT)) {
			vx = -vx;
			if (vx > 0) {
				ghost_koopas->SetSpeed(vx, vy);
				ghost_koopas->SetPos(this->x + 17, y+((TROOPA_BBOX_HEIGHT-GHOST_KOOPAS_BBOX_HEIGHT) / 2) );
			}
			else if (vx < 0) {
				ghost_koopas->SetSpeed(vx, vy);
				ghost_koopas->SetPos(this->x - 17, y + ((TROOPA_BBOX_HEIGHT - GHOST_KOOPAS_BBOX_HEIGHT) / 2));
			}
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	if (koopa_type == KOOPAS_TYPE_RED) {

		int	aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;


		if ((state == TROOPA_STATE_DIE || state == TROOPA_STATE_DIE_UP) && vx == 0) {
			
				
					if (timeReborn->Timeleft() > 4000) {

						aniId = ID_ANI_RED_TROOPA_DIE_DOWN_SHAKE;
					}
					else aniId = ID_ANI_RED_TROOPA_DIE_DOWN_IDLE;


		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
			
				aniId = ID_ANI_RED_TROOPA_DIE_DOWN_RUN;
		}
		else if (state == TROOPA_STATE_DIE_UP) {
			aniId = ID_ANI_RED_TROOPA_DIE_UP_IDLE;
		}
		else if (vx > 0) aniId = ID_ANI_RED_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	
	else/* if(koopa_type == KOOPAS_TYPE_GREEN)*/ {
		int aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if ((state == TROOPA_STATE_DIE || state == TROOPA_STATE_DIE_UP) && vx == 0) {
			
			
					if (timeReborn->Timeleft() > 4000) {

						aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_SHAKE;
					}
					else aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_IDLE;

		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
			
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_RUN;
		}
		
		else if (vx > 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	if (this->state == TROOPA_STATE_ROLL_LEFT || this->state == TROOPA_STATE_ROLL_RIGHT) {
		return;
	}
	CGameObject::SetState(state);

	switch (state)
	{
	case TROOPA_STATE_DIE:
		y += ((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2) - 20;
		vx = 0;
		vy = 0;
		ax = 0;
		//ay = 0;
		//timeStartJump->Stop();

		timeReborn->Start();

		break;
	case TROOPA_STATE_WALKING:
		
		vx = TROOPA_WALKING_SPEED;
		break;
	case TROOPA_STATE_ROLL_LEFT:
		vx = -TROOPA_ROLLING_SPEED;
		break;
	case TROOPA_STATE_ROLL_RIGHT:
		vx = TROOPA_ROLLING_SPEED;
		break;
	}
}
