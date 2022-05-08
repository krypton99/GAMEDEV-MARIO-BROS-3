#pragma once
#include "GameObject.h"

#define GHOST_GRAVITY 0.002f
#define GHOST_STATE_STICK 100

#define GHOST_BBOX_WIDTH 16
#define GHOST_BBOX_HEIGHT 1
#define GHOST_BBOX_HEIGHT_DIE 7

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGhost : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;

public:
	CGhost(float x, float y,
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


class CGhostKoopas : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);


	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGhostKoopas(float x, float y);
	void SetPos(float x, float y) { this->x = x; this->y = y; }
	virtual void SetState(int state);
	bool isOnGround = false;
	virtual void Render();
};
