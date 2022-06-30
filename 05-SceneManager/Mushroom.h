#pragma once
#include "Item.h"
#include "GameObject.h"
#define GREEN_MUSHROOM	2
#define RED_MUSHROOM  1
class CMushroom : public Item
{
private:
	float ax;
	float ay;
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:

	CMushroom(float x, float y, float mushroomType);
	~CMushroom() {}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
