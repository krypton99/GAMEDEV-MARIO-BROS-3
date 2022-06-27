#include "SelectArrow.h"
#include "Mario.h"

void CSelectArrow::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_SELECT_ARROW)->Render(x, y);

	//RenderBoundingBox();
}
CSelectArrow::CSelectArrow(float x, float y,  int sceneId) :CGameObject(x, y)
{
	canDown = true;
	canUp = false;
	this->sceneId = sceneId;
	this->type = OBJECT_TYPE_SELECTARROW;
}

void CSelectArrow::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}