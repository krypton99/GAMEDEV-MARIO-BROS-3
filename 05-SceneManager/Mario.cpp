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
#include "Leaf.h"
#include "FireBullet.h"
#include "VenusFireTrap.h"
#include "Pswitch.h"
#include "Funnel.h"
#include "EndSceneItems.h"
void CMario::UpdateWorldMap(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += ay * dt;
	vx += ax * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * (dt / ((1 + abs(vx * 30))));
	if (stage != WORLD_MAP_SCENE) {
		ghost_mario->SetNx(nx);
	}

	if (getInPipe->Timeleft() < 1000 && getInPipe->Timeleft() > 0) {
		if (funnel->GetIsEntry() && isInPipe) {
			if (funnel->GetDirection()) {
				if (level == MARIO_LEVEL_SMALL) {
					y -= 0.6f;
				} else 
				y -= 0.86f;

				isInHiddenMap = true;
			}
			else {
				if (level == MARIO_LEVEL_SMALL) {
					y += 0.6f;
				}
				else
				y += 0.86f;
				isInHiddenMap = false;
			}
			ay = 0;
			vy = 0;
		}
		else if (funnel->GetIsEntry() && !isInPipe) {
			isOutPipe = true;
			if (funnel->GetDirection()) {
				if (level == MARIO_LEVEL_SMALL) {
					y -= 0.6f;
				}
				else y -= 0.86f;
				isInHiddenMap = false;
			}
			else {
				if (level == MARIO_LEVEL_SMALL) {
					y += 0.6f;
				}
				else y += 0.86f;
				isInHiddenMap = true;
			}
			ay = 0;
			vy = 0;
		
		}
	}
	if (getInPipe->GetStartTime() != 0 && getInPipe->IsTimeUp()) {
		if (funnel->GetDirection()) {
			if (isOutPipe) {
				getInPipe->Stop();
				isOutPipe = false;
				isInPipe = false;
				ay = MARIO_GRAVITY;
				return;
			} else { getInPipe->Start(); }
			isOutPipe = true;
			isInPipe = false;
		}
		else {
			if (isInPipe) {
				getInPipe->Start();
				
			}
			else { 
			getInPipe->Stop();
			isOutPipe = false;
			isInPipe = false;
			ay = MARIO_GRAVITY;
			return;
			}
			isOutPipe = false;
			isInPipe = true;
		}
		
		ay = MARIO_GRAVITY;
		SetState(MARIO_STATE_TELEPORT);
	}
	//DebugOut(L"state %f \n", state);
	if (stage==WORLD_MAP_SCENE) {
		if (abs(vy) > abs(maxVy)) vy = maxVy;
	}
	if (abs(vx) > abs(maxVx)) {
		vx = maxVx;
		if ((state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT) && isOnPlatform)
		{
			isPowerUp = true;
			PowerUp->Start();
			if (level == MARIO_LEVEL_RACOON) {
				flyTime->Start();
				isFlying = true;
			}
		}
	}
	else {
		if (PowerUp->GetStartTime() != 0 && PowerUp->IsTimeUp() && level==MARIO_LEVEL_RACOON) {
			isPowerUp = false;
		}
		else if (level != MARIO_LEVEL_RACOON) {
			isPowerUp = false;
		}
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	isOnPlatform = false;
	
	if (PowerUp->GetStartTime() != 0 && PowerUp->IsTimeUp()) {
		PowerUp->Stop();
		isPowerUp = false;
		//ay = MARIO_GRAVITY;

	}
	if (flyTime->GetStartTime() != 0 && flyTime->IsTimeUp()) {
		flyTime->Stop();
		isFlying = false;
		ay = MARIO_GRAVITY;		
	}
	/*if (vy > 0) {
		isFlying = false;
	}*/
	if (flyStep->Timeleft() > 0 && flyStep->GetStartTime() != 0) {
		ay = 0;
	}
	if (flyStep->GetStartTime() != 0 && flyStep->IsTimeUp()) {
		flyStep->Stop();
		ay = MARIO_GRAVITY;
	}
	if (shell != nullptr) {
		if (ghost_mario->GetHoldingShell() == true && (shell->GetState() == TROOPA_STATE_DIE || shell->GetState() == TROOPA_STATE_DIE_UP)){
			isHolding = true;
			shell->SetIsMariohold(true);
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
		} else if (ghost_mario->GetHoldingShell() == false) {
			isHolding = false;

			if (shell->GetIsMariohold() == true) {
				shell->SetIsMariohold(false);
				if (nx > 0)
				{
					shell->SetState(TROOPA_STATE_ROLL_RIGHT);
				}
				else shell->SetState(TROOPA_STATE_ROLL_LEFT);
			}

		}
	}
	
	
	if (level == MARIO_LEVEL_RACOON) {
		if (isAttack && GetTickCount64() - attackStart->GetStartTime() <= MARIO_TIME_ATTACK) {
			//this->state = MARIO_STATE_ATTACK;
			tail->SetState(TAIL_STATE_HIT);

			// Xet lai huong cho tail khi o frame 2
			if (GetTickCount64() - attackStart->GetStartTime() < MARIO_TIME_ATTACK / 2 && changeNx == 0) {
				nx = -nx;
				changeNx++;
			}
			if (GetTickCount64() - attackStart->GetStartTime() >= MARIO_TIME_ATTACK / 2 && changeNx == 1) {
				changeNx = 0;
				nx = -nx;
			}
			tail->SetPosition(x+5*nx, y);
			tail->Update(dt, coObjects, { x + 5 * nx,y }, nx);

		}
		else {
			if (!isOnPlatform) {
				if (isFlying && !flyTime->IsTimeUp()) {
					this->state = MARIO_STATE_FLY;
				}
				else {
					isFlying = false;
					this->state = MARIO_STATE_JUMP;
				}
			}
			isAttack = false;
			attackStart->Stop();
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMario::DecreaseSpeed() {
	if (vx < 0 && nx == 1 && lastnx == 1) {
		vx = 0;
		ax = 0;
	}
	else if (vx > 0 && nx == -1 && lastnx == -1) {
		vx = 0;
		ax = 0;
	}
	else if (vx > 0 && nx == 1 && lastnx == 1) {
		ax = -MARIO_ACCEL_STOP_X;
		DebugOut(L"ax %f \n", ax);
	}
	else if (vx > 0 && nx == 1 && lastnx == -1) {
		ax = -MARIO_ACCEL_STOP_X;
	}
	
	else if (vx < 0 && nx == -1 && lastnx == -1) {
		ax = MARIO_ACCEL_STOP_X;
	}
	else if (vx < 0 && nx == -1 && lastnx == 1)
	{
		ax = MARIO_ACCEL_STOP_X;
	}
	else {
		vx = 0;
		ax = 0;
		
	}
	//ay = MARIO_GRAVITY;
	//isFlying = false;
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
		/*ax = 0;*/
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj)) {
		if (stage == WORLD_MAP_SCENE) {
			OnCollisionWithPortal(e);
		}
	}
	else if (dynamic_cast<CGhost*>(e->obj))
		OnCollisionWithGhostKoopas(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CFireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
	else if (dynamic_cast<CVenusFireTrap*>(e->obj))
		OnCollisionWithVenus(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CPswitch*>(e->obj))
		OnCollisionWithPswitch(e);
	else if (dynamic_cast<CFunnel*>(e->obj))
	{
		OnCollisionWithFunnel(e);
	}
	else if (dynamic_cast<CEndSceneItems*>(e->obj)) {
		OnCollisionWithEndSceneItems(e);
	}
	
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
void CMario::OnCollisionWithEndSceneItems(LPCOLLISIONEVENT e) {
	CEndSceneItems* endsceneitem = dynamic_cast<CEndSceneItems*>(e->obj);
	CAnimations* animations = CAnimations::GetInstance();
	CAnimationFrame* animationsFrame;
	//endsceneitem->
	int frame = 0;
	if (animations->Get(ID_ANI_ITEM_SHUFFLE)->getCurrentFrame() != NULL) {
		frame = animations->Get(ID_ANI_ITEM_SHUFFLE)->getCurrentFrame();
		animationsFrame = animations->Get(ID_ANI_ITEM_SHUFFLE)->getFrames().at(frame);
		int aniId= animationsFrame->GetSprite()->GetId();
		cards.push_back(aniId);
	}
}
void CMario::OnCollisionWithFunnel(LPCOLLISIONEVENT e) {
	CFunnel* funnel = dynamic_cast<CFunnel*>(e->obj);
	if (funnel->GetIsEntry()) {
		if (funnel->GetDirection()) {
			isInHiddenMap = !this->isInHiddenMap;
			desX = funnel->GetDesX();
			desY= funnel->GetDesY();
			isInPipe = true;
			this->funnel = funnel;
			getInPipe->Start();
		}
		else if (!funnel->GetDirection() && this->canGoThroughPipe_down) {

			isInHiddenMap = !this->isInHiddenMap;
			desX = funnel->GetDesX();
			desY = funnel->GetDesY();
			isInPipe = true;
			this->funnel = funnel;
			getInPipe->Start();
			//state = MARIO_STATE_TELEPORT;
		}
	}
	
}
void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e) {
	CLeaf* item = dynamic_cast<CLeaf*>(e->obj);
	if (item->GetItemType() == ITEM_LEAF) {
		if (level > MARIO_LEVEL_BIG)
		{
			item->SetState(STATE_ERASE);
			return;
		}
		else
		{
			level = MARIO_LEVEL_RACOON;
			item->SetState(STATE_ERASE);
		}
	}
}
void CMario::OnCollisionWithGhostKoopas(LPCOLLISIONEVENT e) {

	CGhost* ghost_koopas = dynamic_cast<CGhost*>(e->obj);
	/*if (holding == true) {
		isHolding = true;
		shell->isMariohold = true;
	}*/
}

void CMario::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if(platform->GetPlatformType()==PLATFORM_TYPE_DEATHZONE)
	{
		SetState(MARIO_STATE_DIE);
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetType() == GOOMBA_TYPE_RED_WING) {

		if (e->ny < 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				goomba->SetType(GOOMBA_TYPE_RED);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_RACOON)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
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
				if (level == MARIO_LEVEL_RACOON)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				} else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithVenus(LPCOLLISIONEVENT e)
{
	CVenusFireTrap* venus = dynamic_cast<CVenusFireTrap*>(e->obj);
	if (level == MARIO_LEVEL_RACOON)
	{
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
	}
	else if (level == MARIO_LEVEL_BIG)
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
void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
	CFireBullet* bullet = dynamic_cast<CFireBullet*>(e->obj);
	if (level == MARIO_LEVEL_RACOON)
	{
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
	}
	else if (level == MARIO_LEVEL_BIG)
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
				if (level == MARIO_LEVEL_RACOON)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
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
			
				if (e->nx <= 0) {
					koopas->SetState(TROOPA_STATE_ROLL_RIGHT);
				}
				else { 
					koopas->SetState(TROOPA_STATE_ROLL_LEFT); 
				}
		}
		else  // hit by Koopas
		{
			if (untouchable == 0)
			{
				if (koopas->GetState() != TROOPA_STATE_DIE)
				{
					if (level == MARIO_LEVEL_RACOON)
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
					else if (level == MARIO_LEVEL_BIG)
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
void CMario::OnCollisionWithPswitch(LPCOLLISIONEVENT e) {
	CPswitch* pswitch = dynamic_cast<CPswitch*>(e->obj);
	if (e->ny < 0) {
		pswitch->SetState(SWITCH_STATE_OFF);
	}
}
void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	AddScore(100);
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
	
	CPortal* portal = dynamic_cast<CPortal*>(e->obj);
	this->mLeft = portal->getMLeft();
	this->mRight = portal->getMRight();
	this->mUp = portal->getMUp();
	this->mDown = portal->getMDown();
	/*CPortal* p = (CPortal*)e->obj;*/
	if (portal->IsHasPortal())
	{
		CGame::GetInstance()->InitiateSwitchScene(portal->GetSceneId());
	}
	
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform && !isHolding && !isInPipe && !isOutPipe)
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
		}
		else if (isInPipe || isOutPipe) {
			aniId = ID_ANI_MARIO_SMALL_PIPE;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0 && nx < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X && isPowerUp)  {
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					else  aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && nx > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && isPowerUp) {
					if (nx < 0) aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					else  aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_RUN_X) {
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
	if (!isOnPlatform && !isHolding && !isInPipe && !isOutPipe)
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
		else if (isInPipe || isOutPipe) {
			aniId = ID_ANI_MARIO_PIPE;
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
				else if (ax == MARIO_ACCEL_RUN_X && isPowerUp) {
					if (nx > 0) aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else  aniId = ID_ANI_MARIO_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_MARIO_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && nx > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && isPowerUp) {
					if (nx < 0) aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else  aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_RUN_X) {
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
	if (!isOnPlatform && !isHolding && !isAttack && !isInPipe && !isOutPipe)
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
				if (isFallLower) {
					aniId = ID_ANI_MARIO_RACOON_FALL_SLOW_RIGHT;
				}
				else
					aniId = ID_ANI_RACOON_MARIO_JUMP_WALK_RIGHT;
			}
			else {
				if (isFallLower) {
					aniId = ID_ANI_MARIO_RACOON_FALL_SLOW_LEFT;
				}
				else
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
		else if (isAttack)
		{
			
				if (temp_nx > 0)
					aniId = ID_ANI_RACOON_MARIO_ATTACK_RIGHT;
				else
					aniId = ID_ANI_RACOON_MARIO_ATTACK_LEFT;
		
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
		
		else if (isInPipe || isOutPipe) {
			aniId = ID_ANI_MARIO_RACOON_PIPE;
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
				else if (ax == MARIO_ACCEL_RUN_X && isPowerUp) {
					if (nx > 0) aniId = ID_ANI_RACOON_MARIO_RUNNING_RIGHT;
					else  aniId = ID_ANI_RACOON_MARIO_RUNNING_LEFT;
				}
				else if (ax <= MARIO_ACCEL_RUN_X) {
					if (nx > 0) aniId = ID_ANI_RACOON_MARIO_WALKING_RIGHT;
					else aniId = ID_ANI_RACOON_MARIO_WALKING_LEFT;
				}
			}
			else if (vx < 0)// vx < 0
			{

				if (ax > 0 && nx > 0)
					aniId = ID_ANI_RACOON_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X && isPowerUp) {
					if (nx < 0) aniId = ID_ANI_RACOON_MARIO_RUNNING_LEFT;
					else  aniId = ID_ANI_RACOON_MARIO_RUNNING_RIGHT;
				}
				else if (ax >= -MARIO_ACCEL_RUN_X) {
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
	if (stage == WORLD_MAP_SCENE) {
		if (GetLevel() == MARIO_LEVEL_RACOON)
			aniId = ID_ANI_MARIO_RACOON_WORLDMAP;
		else if (GetLevel() == MARIO_LEVEL_BIG)
			aniId = ID_ANI_MARIO_BIG_WORLDMAP;
		else
			aniId = ID_ANI_MARIO_SMALL_WORLDMAP;

	} else
	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	tail->SetAni(aniId);
	animations->Get(aniId)->Render(x, y);
	
	RenderBoundingBox();
	
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
		lastnx = nx;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		lastnx = nx;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) > MARIO_WALKING_SPEED + 0.07f)
				vy = -(abs(vx) * 3.0f);
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;
	case MARIO_STATE_RELEASE_FLY:
		if (ay < 0) //ay = MARIO_GRAVITY ;
			break;
	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
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
		//ax=0;

		if (stage == WORLD_MAP_SCENE) {
			ax = 0.0f;
			vx = 0.0f;
		}
		else DecreaseSpeed();
		//isFlying = false;
		break;
	case MARIO_STATE_TELEPORT:
		x = desX;
		y = desY;
		isInPipe = false;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_FLY:
		if (isSitting) break;
		if (level == MARIO_LEVEL_RACOON) {
			ay = 0;
			if (nx > 0)
				vx = MARIO_WALKING_SPEED;
			else
				vx = -MARIO_WALKING_SPEED;
			vy = -MARIO_JUMP_SPEED_Y * 0.7;
			isOnPlatform = false;

		}
		break;
	case MARIO_STATE_ATTACK:
		if (isSitting||isFlying) break;
		temp_nx = nx;
		isAttack = true;
		attackStart->Start();
		tail->SetPosition(x, y);
		tail->SetIsAttack(true);
		break;
	case MARIO_STATE_IDLE_WORLD_MAP:
		ax = 0;
		vx = 0;
		ay = 0;
		vy = 0;
		break;
	case MARIO_STATE_WALKING_UP_WORLD_MAP:
		maxVy = -MARIO_WALKING_SPEED/2;
		ay = -MARIO_ACCEL_WALK_X;
		break;
	case MARIO_STATE_WALKING_DOWN_WORLD_MAP:
		maxVy = MARIO_WALKING_SPEED/2;
		ay = MARIO_ACCEL_WALK_X;
		break;
	case MARIO_STATE_WALKING_RIGHT_WORLD_MAP:
		maxVx = MARIO_WALKING_SPEED/2;
		ax = MARIO_ACCEL_WALK_X;
		break;
	case MARIO_STATE_WALKING_LEFT_WORLD_MAP:
		maxVx = -MARIO_WALKING_SPEED/2;
		ax = -MARIO_ACCEL_WALK_X;
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

