#include "Funnel.h"

void CFunnel::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int ani = 0;
	switch (funnelType) {
	case FUNNEL_TYPE_LONG:
		ani = ID_ANI_FUNNEL;
		break;
	case FUNNEL_TYPE_SHORT:
		ani = ID_ANI_SHORT_FUNNEL;
		break;
	case FUNNEL_TYPE_BLACK:
		ani = ID_ANI_BLACK_FUNNEL;
		break;
	case FUNNEL_TYPE_SHORT_WOOD:
		ani = ID_ANI_SHORT_WOOD_FUNNEL;
		break;
	case FUNNEL_TYPE_LONG_WOOD:
		ani = ID_ANI_LONG_WOOD_FUNNEL;
		break;
	}
	animations->Get(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CFunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (funnelType == FUNNEL_TYPE_SHORT_WOOD || funnelType == FUNNEL_TYPE_LONG) {
		l = x - FUNNEL_BBOX_WIDTH / 2;
		t = y - FUNNEL_BBOX_HEIGHT / 2;
		r = l + FUNNEL_BBOX_WIDTH;
		b = t + FUNNEL_BBOX_HEIGHT;
	}
	else if (funnelType == FUNNEL_TYPE_LONG_WOOD) {
		l = x - FUNNEL_LONG_WOOD_BBOX_WIDTH / 2;
		t = y - FUNNEL_LONG_WOOD_BBOX_HEIGHT / 2;
		r = l + FUNNEL_LONG_WOOD_BBOX_WIDTH;
		b = t + FUNNEL_LONG_WOOD_BBOX_HEIGHT;
	}
	else {
		l = x - FUNNEL_SMALL_BBOX_WIDTH / 2;
		t = y - FUNNEL_SMALL_BBOX_HEIGHT / 2;
		r = l + FUNNEL_SMALL_BBOX_WIDTH;
		b = t + FUNNEL_SMALL_BBOX_HEIGHT;
	}
}