#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define ID_ANI_BRICK 10000
#define ID_ANI_QUESTION_BRICK_ACTIVE 11001
#define ID_ANI_QUESTION_BRICK_EMPTY 11002

#define BRICK_TYPE_QUESTION 100
#define BRICK_TYPE_HIDDEN 200

#define BRICK_STATE_ACTIVE 1
#define BRICK_STATE_EMPTY 0
#define BRICK_STATE_BOUND 2

#define CONTAIN_NONE			0
#define CONTAIN_MUSHROOM		1 
#define CONTAIN_LEAF	2

class CBrick : public CGameObject {
private:
	float brickType;
	int itemType;
	bool isFallingItem = false;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
	
public:
	
	CBrick(float x, float y, float type, int itemType);
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetBrickType() { return brickType; };
	void SetBrickType(float brickType) { this->brickType = brickType; }
	int GetItemType() { return itemType; };
	void SetItemType(int itemType) { this->itemType = itemType; };
	bool GetIsFallingItem() { return isFallingItem; };
	void SetIsFallingItem(bool isFallingItem) { this->isFallingItem = isFallingItem; };
	float GetPosX() { return x; };
	float GetPosY() { return y; };
	virtual void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; };
};