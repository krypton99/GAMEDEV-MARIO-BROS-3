#include "MarioTail.h"
#include "Mario.h"
#include "Utils.h"
#include "Goomba.h"

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
	CGameObject::Update(dt);
	x += vx * dt;
	y += vy * dt;
	this->x = playerPos.x;
	this->y = playerPos.y;
	if (ani) {
		CAnimations* animations = CAnimations::GetInstance();
		int frame = 0;
		if (animations->Get(ani)->getCurrentFrame() != NULL) {
			animations->Get(ani)->getFrames();
			frame = animations->Get(ani)->getCurrentFrame();
		}
		if ((frame == 3||frame == 1) && isAttack) {
			if (nx > 0) {
				this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
			}
			else
				this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
		}
		/*else
			if (frame == 1 && isAttack) {
				if (nx > 0) {
					this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
				}
				else this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
			}*/
		else {
			if (nx > 0) {
				this->x = x - MARIO_RACOON_BBOX_WIDTH / 2;
			}
			else this->x = x + MARIO_RACOON_BBOX_WIDTH / 2;
		}
	}

	this->y = y + MARIO_RACOON_BBOX_HEIGHT / 4;
	this->nx = nx;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMarioTail::OnNoCollision(DWORD dt)
{
	/*x += vx * dt;
	y += vy * dt;*/
}
void CMarioTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CMarioTail*>(e->obj)) return;
	if (dynamic_cast<CKoopas*>(e->obj) /*&& isAttack*/) {
		OnCollisionWithKoopas(e);
	}

}
void CMarioTail::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	koopas->n = this->nx;
	float x, y;
	koopas->GetPosition(x, y);
	koopas->temp_x = x;
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
			if ((frame == 3 || frame == 1) && isAttack) {
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