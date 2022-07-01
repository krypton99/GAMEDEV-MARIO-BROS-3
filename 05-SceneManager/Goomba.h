#pragma once
#include "GameObject.h"
#include "Timer.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_DIE_DEFLECT_SPEED 0.4f
#define GOOMBA_JUMP_SPEED			0.4f
#define GOOMBA_FLY_SPEED			0.2f

#define GOOMBA_TYPE_BROWN 1000
#define GOOMBA_TYPE_RED 2000
#define GOOMBA_TYPE_RED_WING 3000

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7
#define GOOMBA_BBOX_WING_HEIGHT 18
#define GOOMBA_BBOX_WING_WIDTH 20

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_FLY 300
#define GOOMBA_STATE_DIE_BY_OBJECT 400
#define GOOMBA_STATE_JUMP 500

#define ID_ANI_RED_GOOMBA_FLY 5002
#define ID_ANI_RED_GOOMBA_WALKING 5003
#define ID_ANI_RED_GOOMBA_DIE 5004
#define ID_ANI_RED_GOOMBA_DIE_BY_OBJECT 5006
#define ID_ANI_BROWN_GOOMBA_WALKING 5000
#define ID_ANI_BROWN_GOOMBA_DIE 5001
#define ID_ANI_BROWN_GOOMBA_DIE_BY_OBJECT 5005

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 
	float goomba_type;
	ULONGLONG die_start;
	bool isOnGround = false;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	Timer* timeStartJump = new Timer(2000);
	virtual int IsCollidable() { return (state != GOOMBA_STATE_DIE_BY_OBJECT && state != GOOMBA_STATE_DIE); };
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, float type);
	virtual void SetState(int state);
	float GetGoombaType() { return goomba_type; };
	void SetGoombaType(float goomba_type) { this->goomba_type = goomba_type; };
};