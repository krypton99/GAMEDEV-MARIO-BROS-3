#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BULLET_BBOX_WIDTH	8
#define BULLET_BBOX_HEIGHT	8

#define ID_ANI_BULLET_RIGHT 5901
#define ID_ANI_BULLET_LEFT 5902


#define BULLET_GRAVITY 0.0008f //gravity
#define BULLET_SPEED_X	0.2f 
#define BULLET_SPEED_Y	0.1f 


class CBullet : public CGameObject {
private:
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);

public:
	CBullet(float x, float y, int nx);
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
};