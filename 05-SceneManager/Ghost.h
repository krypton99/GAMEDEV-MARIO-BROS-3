#pragma once
#include "GameObject.h"

#define GHOST_GRAVITY 0.002f
#define GHOST_STATE_STICK 100

#define GHOST_BBOX_WIDTH 16
#define GHOST_BBOX_HEIGHT 1
#define GHOST_BBOX_HEIGHT_DIE 7

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGhost : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;

public:
	CGhost(float x, float y,
		float cell_width, float cell_height, int length) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
	}
	int isHit = 0;
	int IsBlocking() { return 0; }
	/*int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}*/
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	
};
