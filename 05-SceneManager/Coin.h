#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_HIDDEN 100

class CCoin : public CGameObject {
	bool isPswitch = false; // coin in Pswitch
	
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	bool getIsPswitch() { return isPswitch; }
	void setIsPswitch(bool isPswitch) { this->isPswitch = isPswitch; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};