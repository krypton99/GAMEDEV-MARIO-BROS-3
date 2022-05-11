#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "Platform.h"
#include "Ghost.h"
#include "Brick.h"
#include "Mushroom.h"
#include "Koopas.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	
	DebugOut(L"canHold %d \n", canHold);
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (flyTime->IsTimeUp())
		flyTime->Stop();
	isOnPlatform = false;
	if (level == MARIO_LEVEL_RACOON) {
		if (!isOnPlatform) {
			if (isFlying && !flyTime->IsTimeUp()) {
				this->state = MARIO_STATE_FLY;
			}
			else {
				isFlying = false;
				this->state = MARIO_STATE_JUMP;
			}
		}
	}
	if (shell != nullptr) {
		if (isHolding == true) {
			if (level == MARIO_LEVEL_SMALL) {
				if (nx > 0) {
					shell->SetPosition(x + 10, y - 5);
				}
				else shell->SetPosition(x - 10, y - 5);
			}
			else if (level == MARIO_LEVEL_BIG) {
				if (nx > 0) {
					shell->SetPosition(x + 12, y - 5);
				}
				else shell->SetPosition(x - 12, y - 5);
			}
			else {
				if (nx > 0) {
					shell->SetPosition(x + 12, y - 5);
				}
				else shell->SetPosition(x - 12, y - 5);
			}
		}
	}
	if (isHolding == false) {

		if (shell != nullptr) {
			if (shell->isMariohold == true) {
				shell->isMariohold = false;
				if (nx > 0)
				{
					shell->SetState(TROOPA_STATE_ROLL_RIGHT);
				}
				else shell->SetState(TROOPA_STATE_ROLL_LEFT);
			}
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
	
}
void CMario::DecreaseSpeed() {
	if (vx < 0 && nx == 1) {
		vx = 0;
		ax = 0;
	}
	else if (vx > 0 && nx == -1) {
		vx = 0;
		ax = 0;
	}
	else if (vx > 0 && nx == 1) {
		ax = -MARIO_ACCEL_STOP_X;
	}
	else if (vx < 0 && nx == -1) {
		ax = MARIO_ACCEL_STOP_X;
	}
	else {
		vx = 0;
		ax = 0;
		
	}
	isFlying = false;
}
void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	//canHold = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) {
			isOnPlatform = true;
		}
		//canHold = false;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CGhost*>(e->obj))
		OnCollisionWithGhost(e);
	else if (dynamic_cast<CGhostKoopas*>(e->obj))
		OnCollisionWithGhostKoopas(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	/*else 
		canHold = false;*/
	
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e) {
	CMushroom* item = dynamic_cast<CMushroom*>(e->obj);
	if (item->GetItemType() == ITEM_RED_MUSHROOM) {
		if (level > MARIO_LEVEL_SMALL)
		{
			e->obj->SetState(STATE_ERASE);
			return;
		}
		else
		{
			e->obj->SetState(STATE_ERASE);
			SetLevel(MARIO_LEVEL_BIG);

		}
	}
}
void CMario::OnCollisionWithGhostKoopas(LPCOLLISIONEVENT e) {
	canHold = false;
	CGhostKoopas* ghost_koopas = dynamic_cast<CGhostKoopas*>(e->obj);
	/*if (holding == true) {
		isHolding = true;
		shell->isMariohold = true;
	}*/
	canHold = true;
}
void CMario::OnCollisionWithGhost(LPCOLLISIONEVENT e)
{
	CGhost* ghost = dynamic_cast<CGhost*>(e->obj);
	if (e->ny >= 0) {
		ghost->isHit = 1;
	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	shell = koopas;
	// jump on top >> kill Goomba and deflect a bit 
	if (koopas->GetKoopasType() == KOOPAS_TYPE_GREEN_WING) {

		if (e->ny < 0)
		{
			if (koopas->GetState() != TROOPA_STATE_DIE)
			{
				koopas->SetKoopasType(KOOPAS_TYPE_GREEN);
				koopas->SetState(TROOPA_STATE_WALKING);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (untouchable == 0)
		{
			if (koopas->GetState() != TROOPA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}

		}
	}
	else
		if (e->ny < 0)
		{
			if (koopas->GetState() != TROOPA_STATE_DIE)
			{
				koopas->SetState(TROOPA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopas->GetState() == TROOPA_STATE_DIE || koopas->GetState() == TROOPA_STATE_DIE_UP) {
				
				if (e->nx <= 0) {
					koopas->SetState(TROOPA_STATE_ROLL_RIGHT);
				}
				else { koopas->SetState(TROOPA_STATE_ROLL_LEFT); }
			}
		}
		else if (koopas->GetState() == TROOPA_STATE_DIE || koopas->GetState() == TROOPA_STATE_DIE_UP)
		{
			if (holding == true) {
				isHolding = true;
				shell->isMariohold = true;
			}
			else
				if (e->nx <= 0) {
					koopas->SetState(TROOPA_STATE_ROLL_RIGHT);
				}
				else { koopas->SetState(TROOPA_STATE_ROLL_LEFT); }
		}
		else  // hit by Koopas
		{
			if (untouchable == 0)
			{
				if (koopas->GetState() != TROOPA_STATE_DIE)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}

			}
		}
}
void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}
void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 

	//Mario hit Venus
	if (brick->GetBrickType() != BRICK_TYPE_HIDDEN) {
		if (brick->GetState() == BRICK_STATE_ACTIVE) {
			
			if (e->ny > 0) {

				//brick->SetState(BRICK_STATE_EMPTY);
				brick->SetState(BRICK_STATE_BOUND);
			}

		}
	}
	
}
void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform && !isHolding)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isHolding) {
			if (shell->GetKoopasType() == KOOPAS_TYPE_RED) {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_HOLDING_LEFT_RED;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_RED;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_RED;
					else aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_RED;
				}
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_HOLDING_LEFT_GREEN;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_GREEN;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_GREEN;
					else aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_GREEN;
				}
			}
		} else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0 && nx < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					else  aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_WALK_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && nx > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					if (nx < 0) aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					else  aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_WALK_X) {
					if (nx < 0) aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
					else aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform && !isHolding)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isHolding) {
			if (shell->GetKoopasType() == KOOPAS_TYPE_RED) {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_IDLE_HOLDING_LEFT_RED;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_WALK_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_RED;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_WALK_HOLDING_LEFT_RED;
					else aniId = ID_ANI_MARIO_WALK_HOLDING_RIGHT_RED;
				}
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_IDLE_HOLDING_LEFT_GREEN;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_WALK_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_WALK_HOLDING_LEFT_GREEN;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_WALK_HOLDING_LEFT_GREEN;
					else aniId = ID_ANI_MARIO_WALK_HOLDING_RIGHT_GREEN;
				}
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0 && nx < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else  aniId = ID_ANI_MARIO_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_WALK_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && nx > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					if (nx < 0) aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else  aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_WALK_X) {
					if (nx < 0) aniId = ID_ANI_MARIO_WALKING_LEFT;
					else aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}
int CMario::GetAniIdRacoon()
{
	int aniId = -1;
	if (!isOnPlatform && !isHolding)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_RACOON_MARIO_FLY_RIGHT;
			else
				aniId = ID_ANI_RACOON_MARIO_FLY_LEFT;
		}
		else
		{
			if (nx >= 0) {
				/*if (isFallLower) {
					aniId = ID_ANI_MARIO_RACOON_FALL_SLOW_RIGHT;
				}
				else*/
					aniId = ID_ANI_RACOON_MARIO_JUMP_WALK_RIGHT;
			}
			else {
				/*if (isFallLower) {
					aniId = ID_ANI_MARIO_RACOON_FALL_SLOW_LEFT;
				}
				else*/
					aniId = ID_ANI_RACOON_MARIO_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_RACOON_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_RACOON_MARIO_SIT_LEFT;
		}
		
		else if (isHolding) {
			if (shell->GetKoopasType() == KOOPAS_TYPE_RED) {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_RACOON_IDLE_HOLDING_LEFT_RED;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_RED;
					else aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_RED;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_RED;
					else aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_RED;
				}
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_RACOON_IDLE_HOLDING_LEFT_GREEN;
				}
				else if (vx > 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_GREEN;
					else aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_GREEN;
				}
				else {
					if (nx < 0) aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_GREEN;
					else aniId = ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_GREEN;
				}
			}
		}
		
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_RACOON_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_RACOON_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0 && nx < 0)
					aniId = ID_ANI_RACOON_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_RACOON_MARIO_RUNNING_RIGHT;
					else  aniId = ID_ANI_RACOON_MARIO_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_WALK_X) {
					if (nx > 0) aniId = ID_ANI_RACOON_MARIO_WALKING_RIGHT;
					else aniId = ID_ANI_RACOON_MARIO_WALKING_LEFT;
				}
			}
			else if (vx < 0)// vx < 0
			{

				if (ax > 0 && nx > 0)
					aniId = ID_ANI_RACOON_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					if (nx < 0) aniId = ID_ANI_RACOON_MARIO_RUNNING_LEFT;
					else  aniId = ID_ANI_RACOON_MARIO_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_WALK_X) {
					if (nx < 0) aniId = ID_ANI_RACOON_MARIO_WALKING_LEFT;
					else aniId = ID_ANI_RACOON_MARIO_WALKING_RIGHT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_RACOON_MARIO_IDLE_RIGHT;

	return aniId;
}
void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		/*ax = 0.0f;
		vx = 0.0f;*/
		DecreaseSpeed();
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_FLY:
		if (isSitting) break;
		if (level == MARIO_LEVEL_RACOON) {
			isFlying = true;
			ay = -0.0002f;
			vy = 0;
			isOnPlatform = false;
			flyTime->Start();
		}
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if(level==MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else {
		if (isSitting)
		{
			left = x - MARIO_RACOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		if (l == MARIO_LEVEL_BIG) {
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		}
		else y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else if (this->level == MARIO_LEVEL_BIG) {
		y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) / 2;
	}
	level = l;
}

