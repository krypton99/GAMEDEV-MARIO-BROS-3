#pragma once
#include "GameObject.h"
#include "Timer.h"

#define ITEM_LEAF			0
#define ITEM_RED_MUSHROOM		1
#define ITEM_GREEN_MUSHROOM		2

#define ITEM_BBOX 15

#define ITEM_STATE_HIDDEN	0	
#define ITEM_STATE_ABLE		1	

#define ITEM_SPEED_Y	0.05f
#define ITEM_GRAVITY	0.0003f

#define ID_ITEM_ANI_MUSHROOM		9001

#define ID_ITEM_ANI_LEAF_LEFT		9011
#define ID_ITEM_ANI_LEAF_RIGHT		9010

#define ITEM_DISAPPEAR_TIME  5000

class Item : public CGameObject
{
public:
	float ItemType;

	Item();
	~Item() {};

	Timer* disapperTimer = new Timer(ITEM_DISAPPEAR_TIME);
	float GetItemType() { return ItemType; }
	virtual void Render() = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
