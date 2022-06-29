#pragma once
#include "GameObject.h"
#define ID_ANI_ITEM_SHUFFLE 9045
class CEndSceneItems :
	public CGameObject
{
public:
	bool isHit = false;

	CEndSceneItems(float x, float y);

	void Hit() { this->isHit = true; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};