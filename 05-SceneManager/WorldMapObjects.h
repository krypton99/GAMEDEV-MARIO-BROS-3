#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define PLANT_OBJECT	1
#define TEXT_OBJECT		2
#define GUARD_OBJECT	3


#define ANI_PLANT_OBJECT			19001
#define ANI_TEXT_HELP				19002
#define ANI_GUARD_WALK_RIGHT		19003
#define ANI_GUARD_WALK_LEFT			19004

class CWorldMapObjects : public CGameObject {
protected:
	float object_type;
public:
	CWorldMapObjects(float x, float y, float object_type);
	void SetObjectType(float objectType) { this->object_type = objectType; }
	float GetObjectType() { return this->object_type; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlockingX() { return 1; };
	virtual int IsBlockingY() { return 1; };
};