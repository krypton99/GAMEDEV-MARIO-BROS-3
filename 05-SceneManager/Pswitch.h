#pragma once
#include "Item.h"
#include "GameObject.h"
#define SWITCH_STATE_ON		0
#define SWITCH_STATE_OFF	1
#define SWITCH_STATE_INACTIVE	2

#define ID_ITEM_ANI_PSWITCH_ON	9020
#define ID_ITEM_ANI_PSWITCH_OFF	9021

#define PSWITCH_OFF_BBOX 7
class CPswitch : public CGameObject
{
private:

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }
	bool onChange = false;
	Timer* changeTimer = new Timer(12000);
	vector<LPGAMEOBJECT>* coins = nullptr;
public:
	CPswitch(float x, float y);
	~CPswitch() {}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* object);
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

