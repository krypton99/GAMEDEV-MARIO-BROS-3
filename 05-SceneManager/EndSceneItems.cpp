#include "EndSceneItems.h"
#include "AssetIDs.h"

CEndSceneItems::CEndSceneItems(float x, float y) :CGameObject(x, y) {
	this->type = OBJECT_TYPE_LASTITEM;
}

void CEndSceneItems::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

}
void CEndSceneItems::Render()
{
	//0: star || 1: mushroom || 2: flower
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_ITEM_SHUFFLE)->Render(x, y);
}

void CEndSceneItems::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == STATE_ERASE)
		l = t = b = r = 0;
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX;
		b = y + BRICK_BBOX;
	}
}