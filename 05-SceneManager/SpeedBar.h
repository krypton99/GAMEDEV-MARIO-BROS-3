#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define ID_ANI_SPEED_0 9917
#define ID_ANI_SPEED_1 9910
#define ID_ANI_SPEED_2 9911
#define ID_ANI_SPEED_3 9912
#define ID_ANI_SPEED_4 9913
#define ID_ANI_SPEED_5 9914
#define ID_ANI_SPEED_6 9915
#define ID_ANI_SPEED_7 9916

#define	SPEED_0 0
#define SPEED_1 1
#define	SPEED_2 2
#define SPEED_3 3
#define	SPEED_4 4
#define SPEED_5 5
#define	SPEED_6 6
#define SPEED_7 7

class CSpeedBar : public CGameObject {
public:
	CSpeedBar(float x, float y) : CGameObject(x, y) {
	}
	void Render(float vx);
	virtual void Render() {};
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};