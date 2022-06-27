#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define ID_ANI_SELECT_ARROW 9930
#define STEP_DISTANCE 16

class CSelectArrow : public CGameObject {
	int sceneId;
	bool canUp;
	bool canDown;
public:
	CSelectArrow(float x, float y, int sceneId);
	void Render();
	//virtual void Render() {};
	void Update(DWORD dt) {}
	bool GetCanDown() { return this->canDown; }
	int GetSceneId() { return this->sceneId; }
	void SetCanDown(bool canDown) { this->canDown=canDown; }
	bool GetCanUp() { return this->canUp; }
	void SetCanUp(bool canUp) { this->canUp = canUp; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};