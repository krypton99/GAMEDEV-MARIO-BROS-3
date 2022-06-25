#pragma once

#include "GameObject.h"
#define COIN_SPEED_Y	0.2f
class CCoinEffect : public CGameObject {
private:
	bool isBound;
public:

	CCoinEffect(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	
};