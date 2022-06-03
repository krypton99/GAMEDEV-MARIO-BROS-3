#pragma once
#include "GameObject.h"


#define GHOST_GRAVITY 0.002f
#define GHOST_STATE_STICK 100
#define GHOST_TYPE_KOOPAS 1
#define GHOST_TYPE_MARIO 2
#define GHOST_TYPE_SHELL 3

#define GHOST_BBOX_WIDTH 16
#define GHOST_BBOX_HEIGHT 1
#define GHOST_KOOPAS_BBOX_WIDTH 16
#define GHOST_SHELL_BBOX_WIDTH 32
#define GHOST_MARIO_BBOX_WIDTH 16
#define GHOST_KOOPAS_BBOX_HEIGHT 16
#define GHOST_BBOX_HEIGHT_DIE 7


#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGhostPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;

public:

	CGhostPlatform(float x, float y,
		float cell_width, float cell_height, int length) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
	}
	int isHit = 0;
	int IsBlocking() { return 0; }
	/*int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}*/
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	
};


class CGhost : public CGameObject
{
protected:
	float ax;
	float ay;
	
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);


	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	//virtual int IsThrough() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
public:
	bool holdingShell=false;
	int type;
	int nx;
	CGhost(float x, float y);
	void SetPos(float x, float y) { this->x = x; this->y = y; }
	void SetVx(float vx) { this->vx = vx; }
	virtual void SetState(int state);
	bool isOnGround = false;
	virtual void Render();
};
