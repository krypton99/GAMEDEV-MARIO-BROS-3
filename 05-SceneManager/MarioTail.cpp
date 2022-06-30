#include "MarioTail.h"
#include "Mario.h"
#include "Utils.h"
#include "Goomba.h"
#include "Brick.h"
#include "PlayScene.h"
#include "VenusFireTrap.h"
CMarioTail* CMarioTail::__instance = nullptr;

CMarioTail* CMarioTail::GetInstance(float x, float y)
{
	if (__instance == NULL) __instance = new CMarioTail(x, y);
	return __instance;
}
CMarioTail::CMarioTail(float x, float y) :CGameObject(x, y) {
	this->start_x = x;
	this->start_y = y;
}
void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects,D3DXVECTOR2 playerPos, int playerNx) {
	//CGameObject::Update(dt);
	this->nx = playerNx;
	
	
	if (state == TAIL_STATE_HIT) {
		vx = 0.15f * playerNx;
		x += vx * dt;
	}
	
	/*this->x = playerPos.x;
	this->y = playerPos.y;*/
	//if (ani) {
	//	CAnimations* animations = CAnimations::GetInstance();
	//	int frame = 0;
	//	if (animations->Get(ani)->getCurrentFrame() != NULL) {
	//		animations->Get(ani)->getFrames();
	//		frame = animations->Get(ani)->getCurrentFrame();
	//		DebugOut(L"frame %d \n", frame);
	//	}
	//	if ((frame == 3||frame == 1) /*&& isAttack*/) {
	//		if (nx > 0) {
	//			this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;				
	//		}
	//		else {
	//			this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;			
	//		}
	//	}
	//	else {
	//		if (nx > 0) {
	//			this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
	//			
	//		}
	//		else {
	//			this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
	//		}
	//	}
	//}

	this->y = y + MARIO_RACOON_BBOX_HEIGHT / 4;
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMarioTail::OnNoCollision(DWORD dt)
{
	/*x += vx * dt;
	y += vy * dt;*/
}
void CMarioTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	/*if (!e->obj->IsBlocking()) return;*/
	//if (dynamic_cast<CMarioTail*>(e->obj)) return;
	if (dynamic_cast<CKoopas*>(e->obj) /*&& isAttack*/) {
		OnCollisionWithKoopas(e);
	} else
	if (dynamic_cast<CGoomba*>(e->obj) ) {
		OnCollisionWithGoomba(e);
	} else
	if (dynamic_cast<CBrick*>(e->obj) ) {
		OnCollisionWithBrick(e);
	} else
	if (dynamic_cast<CVenusFireTrap*>(e->obj)) {
			OnCollisionWithVenus(e);
	}

}
void CMarioTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	
	goomba->SetState(GOOMBA_STATE_DIE_BY_OBJECT);
}
void CMarioTail::OnCollisionWithVenus(LPCOLLISIONEVENT e) {
	CVenusFireTrap* venus = dynamic_cast<CVenusFireTrap*>(e->obj);
	if (venus->GetState() == VENUS_STATE_UP || venus->GetState() == VENUS_STATE_DOWN)
	{
		venus->SetIsDeleted(true);
	}
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->AddScore(1000);
}
void CMarioTail::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	
	if (brick->GetState() != BRICK_STATE_EMPTY && (brick->GetBrickType() != BRICK_TYPE_GOLD || brick->GetItemType()==CONTAIN_PSWITCH)) {
		brick->SetIsFallingItem(true);
		brick->SetState(BRICK_STATE_EMPTY);
	}
	if (brick->GetBrickType() == BRICK_TYPE_GOLD && brick->GetState() != BRICK_STATE_EMPTY) {
		
		brick->SetState(BRICK_STATE_BROKEN);
	}
}
void CMarioTail::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	float ml, mr, mb, mt;
	float kl, kr, kb, kt;
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetBoundingBox(ml,mt,mr,mb);
	//GetBoundingBox(ml, mt, mr, mb);
	koopas->GetBoundingBox(kl, kt, kr, kb);
	if (kr <= ml) {
		koopas->SetNx(-1);
	}
	else if (kl >= mr) {
		koopas->SetNx(1);
	}
	float x, y;
	koopas->GetPosition(x, y);
	koopas->SetTempX(x);
	koopas->SetState(TROOPA_STATE_DIE_UP);
}
void CMarioTail::GetBoundingBox(float& l, float& t, float& r, float& b) {

	if (state == TAIL_STATE_HIT) {
		if (ani) {
			CAnimations* animations = CAnimations::GetInstance();
			int frame = 0;
			if (animations->Get(ani)->getCurrentFrame() != NULL) {
				frame = animations->Get(ani)->getCurrentFrame();
			}
			if ((frame == 3 || frame == 1)/* && isAttack*/) {
				if (nx > 0) {
					l = x + TAIL_BBOX_WIDTH / 2;
				}
				else
					l = x - TAIL_BBOX_WIDTH / 2;
			}
			else {
				if (nx > 0) {
					l = x - TAIL_BBOX_WIDTH / 2;
				}
				else l = x + TAIL_BBOX_WIDTH / 2;
			}
				
				/*else l = x - TAIL_BBOX_WIDTH / 2;*/
			t = y - TAIL_BBOX_HEIGHT / 2;
			r = l + TAIL_BBOX_WIDTH;
			b = t + TAIL_BBOX_HEIGHT;
		}
	}

}
void CMarioTail::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {


	}
}
void CMarioTail::Render() {
	RenderBoundingBox();
}