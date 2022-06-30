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
	timeStartJump->Start();
	isGhostFollow = true;
	type = OBJECT_TYPE_KOOPAS;
	die_start = -1;
	if (koopa_type == KOOPAS_TYPE_RED || koopa_type == KOOPAS_TYPE_GREEN) {
		SetState(TROOPA_STATE_WALKING);
		isGhostFollow = true;
	}
	else {
		SetState(TROOPA_STATE_FLYING);
	}
		
	start_vx = vx;
}
bool CKoopas::CheckDistancePlayer(D3DXVECTOR4 player)
{

	float kl, kt, kr, kb;
	GetBoundingBox(kl, kt, kr, kb);
	if ((abs(player.x - kr) < MARIO_DISTANCE_KOOPAS)|| (abs(player.z - kl) < MARIO_DISTANCE_KOOPAS))
	{
		return true;
	}// trong vung gan rua
		
	return false;
}
int CKoopas::CheckNxShell(D3DXVECTOR4 player)
{
	if ((player.x - x) > 0) 
		return 1;
	return -1;
}
void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE || state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT || state == TROOPA_STATE_DIE_UP)
	{
		left = x - TROOPA_BBOX_WIDTH_DIE / 2;
		top = y - TROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + TROOPA_BBOX_WIDTH_DIE;
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
	/*if (dynamic_cast<CPlatform*>(e->obj)) return;*/
	/*}*/

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) // va cham ground
		{
			isOnGround = true;
			if (isFly) {
				vy = -TROOPA_FLY_SPEED;
			}
		}
	}
	else if (e->nx != 0)
	{
			vx = -vx;
	}
	if (dynamic_cast<CBrick*>(e->obj) && (state==TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT))
	{
		OnCollisionWithBrick(e);
	}
	if(dynamic_cast<CGoomba*>(e->obj))
	{
		OnCollisionWithGoomba(e);
	}
}
void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick->GetState() != BRICK_STATE_EMPTY && brick->GetBrickType() != BRICK_TYPE_GOLD ) {
		brick->SetIsFallingItem(true);
		brick->SetState(BRICK_STATE_EMPTY);
	}
	if(brick->GetBrickType() == BRICK_TYPE_GOLD) {
		brick->SetState(BRICK_STATE_BROKEN);
	}
}
void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
		goomba->SetState(GOOMBA_STATE_DIE_BY_OBJECT);
	}
	
}
void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetBoundingBox(ml, mt, mr, mb);
	D3DXVECTOR4 player(ml, mt, mr, mb);
	canHoldingshell = CheckDistancePlayer(player);
	vy += ay * dt;
	vx += ax * dt;
	if (isMariohold) {
		ay = 0;
	}
	else ay = TROOPA_GRAVITY;
	if (koopa_type == KOOPAS_TYPE_GREEN_WING && state != TROOPA_STATE_DIE) {
		if (timeStartJump->IsTimeUp() && timeStartJump->GetStartTime()) { // bd tinh time nhay
			timeStartJump->Stop();
			SetState(TROOPA_STATE_JUMP);
			timeStartJump->Start();
		}
	}
	if (state == TROOPA_STATE_DIE) {
		if (timeReborn->IsTimeUp() && timeReborn->GetStartTime()) { // bd tinh time hoi sinh
			timeReborn->Stop();
			if (!isMariohold) {
				SetState(TROOPA_STATE_WALKING);
				y -= 10;
				if (koopa_type == KOOPAS_TYPE_RED) {
					ghost_koopas->SetGhostType(GHOST_TYPE_KOOPAS);
					ghost_koopas->SetPosition(x + 16, y);
					ghost_koopas->SetSpeed(vx, vy);
				}
			}
		}
	}
	if (state == TROOPA_STATE_DIE_UP) {
		/*if (nx > 0) {
			if (x > temp_x + SHELL_THROW_DISTANCE_X) {
				vx = 0;

			}
		}
		else {
			if (x < temp_x - SHELL_THROW_DISTANCE_X) {
				vx = 0;

			}
		}*/
		if (x > temp_x + SHELL_THROW_DISTANCE_X || x < temp_x - SHELL_THROW_DISTANCE_X) {
			vx = 0;

		}
	}
	if (koopa_type == KOOPAS_TYPE_RED) {
		if (ghost_koopas->GetIsOnGround() == false ) {
			vx = -vx;
		}
		if (state != TROOPA_STATE_DIE && state != TROOPA_STATE_ROLL_LEFT && state != TROOPA_STATE_ROLL_RIGHT && state != TROOPA_STATE_DIE_UP) {
			if (vx > 0) {
				ghost_koopas->SetSpeed(vx, vy);
				ghost_koopas->SetPos(this->x + 17, y + ((TROOPA_BBOX_HEIGHT - GHOST_KOOPAS_BBOX_HEIGHT) / 2));
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

		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if ((state == TROOPA_STATE_DIE || state == TROOPA_STATE_DIE_UP) && vx == 0) {
			if (isMariohold) {
				RenderBoundingBox();
				return;
			}
			else
				if (state == TROOPA_STATE_DIE_UP) {
					aniId = ID_ANI_RED_TROOPA_DIE_UP_IDLE;
				}
				else {
					if (timeReborn->Timeleft() > 4000) {

						aniId = ID_ANI_RED_TROOPA_DIE_DOWN_SHAKE;
					}
					else aniId = ID_ANI_RED_TROOPA_DIE_DOWN_IDLE;

				}

		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
			if (isShellUp) {
				aniId = ID_ANI_RED_TROOPA_DIE_UP_RUN;
			}
			else
				aniId = ID_ANI_RED_TROOPA_DIE_DOWN_RUN;
		}
		else if (state == TROOPA_STATE_DIE_UP) {
			aniId = ID_ANI_RED_TROOPA_DIE_UP_IDLE;
		}
		else if (vx > 0) aniId = ID_ANI_RED_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_RED_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else if (koopa_type == KOOPAS_TYPE_GREEN_WING) {

		int aniId = 0;
		if (vx > 0) {
			aniId = ID_ANI_GREEN_TROOPA_FLY_RIGHT;
		}
		else if (vx < 0) {
			aniId = ID_ANI_GREEN_TROOPA_FLY_LEFT;
		}

		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	else if(koopa_type == KOOPAS_TYPE_GREEN) {
		int aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		//int ani = KOOPA_TROOPA_ANI_WALKING_LEFT;
		if ((state == TROOPA_STATE_DIE || state == TROOPA_STATE_DIE_UP) && vx == 0) {
			if (isMariohold) {
				RenderBoundingBox();
				return;
			}
			else
				if (state == TROOPA_STATE_DIE_UP) {
					aniId = ID_ANI_GREEN_TROOPA_DIE_UP_IDLE;
				}
				else {
					if (timeReborn->Timeleft() > 4000) {

						aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_SHAKE;
					}
					else aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_IDLE;

				}
		}
		else if (state == TROOPA_STATE_ROLL_LEFT || state == TROOPA_STATE_ROLL_RIGHT) {
			if (isShellUp) {
				aniId = ID_ANI_GREEN_TROOPA_DIE_UP_RUN;
			}
			aniId = ID_ANI_GREEN_TROOPA_DIE_DOWN_RUN;
		}
		else if (state == TROOPA_STATE_DIE_UP) {
			aniId = ID_ANI_GREEN_TROOPA_DIE_UP_IDLE;
		}
		else if (vx > 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_GREEN_TROOPA_WALKING_LEFT;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	if ((this->state == TROOPA_STATE_ROLL_LEFT || this->state == TROOPA_STATE_ROLL_RIGHT) && state!= TROOPA_STATE_DIE_UP) {
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
		//ghost_koopas->SetVx(0);
		//ghost_koopas->type = GHOST_TYPE_SHELL;
		ghost_koopas->SetPosition(x, y + 5);
		//ay = 0;
		//timeStartJump->Stop();
		isShellUp = false;
		timeReborn->Start();

		break;
	case TROOPA_STATE_WALKING:
		isFly = false;
		vx = -TROOPA_WALKING_SPEED;
		break;
	case TROOPA_STATE_ROLL_LEFT:
		vx = -TROOPA_ROLLING_SPEED;
		break;
	case TROOPA_STATE_DIE_UP:
		
		vy = -0.2f;
		ax = 0;
		if (nx > 0) {
			vx = 0.05f;
		}
		else {
			vx = -0.05f;
		}

		isShellUp = true;
		break;
	case TROOPA_STATE_ROLL_RIGHT:
		vx = TROOPA_ROLLING_SPEED;
		break;
	case TROOPA_STATE_JUMP:
		isOnGround = false;
		vy = -TROOPA_JUMP_SPEED;
		break;
	case TROOPA_STATE_FLYING:
		isFly = true;
		vx = -0.05f;
		break;
	}
}
