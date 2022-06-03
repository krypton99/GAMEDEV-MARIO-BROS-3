#pragma once
#include "Item.h"
#include "GameObject.h"

#define LEAF_SPEED_FALL	0.03f
#define LEAF_SPEED_X	0.08f

class CLeaf : public Item
{
private:
	float ax;
	float ay;
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };
	virtual void OnNoCollision(DWORD dt);
	BOOLEAN isFalling = false;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CLeaf(float x, float y, float leafType);
	~CLeaf() {}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
