#include "WorldMapObjects.h"
#include "AssetIDs.h"
#include "Grid.h"
#include "Brick.h"

CWorldMapObjects::CWorldMapObjects(float x, float y, float object_type) :CGameObject(x, y) {
	this->object_type = object_type;
	this->type = OBJECT_TYPE_WORLDMAPOBJ;

}

void CWorldMapObjects::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = 0;
	if (GetObjectType() == PLANT_OBJECT)
		aniId = ANI_PLANT_OBJECT;
	else if (GetObjectType() == TEXT_OBJECT)
		aniId = ANI_TEXT_HELP;
	else
	{
		if (vx < 0)
			aniId = ANI_GUARD_WALK_LEFT;
		else
			aniId = ANI_GUARD_WALK_RIGHT;
	}
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CWorldMapObjects::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	CGameObject::Update(dt);
	if (GetObjectType() == GUARD_OBJECT)
	{
		x += vx * dt;
		if (x > start_x + BRICK_BBOX || x < start_x)
		{
			if (x > start_x + BRICK_BBOX)
				x = start_x + BRICK_BBOX;
			if (x < start_x)
				x = start_x;
			vx = -vx;
		}
	}

}
void CWorldMapObjects::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}