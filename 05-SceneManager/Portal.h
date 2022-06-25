#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	bool mLeft;
	bool mRight;
	bool mUp;
	bool mDown;
	float width;
	float height; 
	bool hasPortal; // 1: true
public:
	CPortal(bool hasPortal, float l, float t, float r, float b, int scene_id, bool mLeft, bool mRight, bool mUp, bool mDown);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	bool getMLeft() { return this->mLeft; }
	bool getMRight() { return this->mRight; }
	bool getMUp() { return this->mUp; }
	bool getMDown() { return this->mDown; }
	void RenderBoundingBox(void);
	bool IsHasPortal() { return this->hasPortal; }
	int GetSceneId() { return scene_id;  }
	int IsBlocking() { return 0; }
};