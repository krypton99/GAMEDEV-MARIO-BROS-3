#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define TAIL_STATE_HIT			0

#define TAIL_ANI_RIGHT			0	
#define TAIL_ANI_LEFT			1

#define TAIL_BBOX_HEIGHT		6
#define TAIL_BBOX_WIDTH			6
#define DISTANCE_XTAIL_MARIO	7

class CMarioTail : public CGameObject {
private:
	static CMarioTail* __instance;
	int ani;
	BOOLEAN isAttack = false;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithVenus(LPCOLLISIONEVENT e);
public:
	CMarioTail(float x, float y);
	void Render();
	void Update(DWORD dt) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 playerPos, int playerNx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetPosX() { return x; };
	float GetPosY() { return y; };
	virtual void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	static CMarioTail* GetInstance(float x, float y);
	bool GetIsAttack() { return isAttack; }
	void SetIsAttack(bool isAttack) { this->isAttack = isAttack; }
	int GetAni() { return ani; }
	void SetAni(int ani) { this->ani = ani; }
};
