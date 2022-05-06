#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define ID_ANI_FUNNEL 12001
#define ID_ANI_SHORT_FUNNEL 12002

#define FUNNEL_BBOX_WIDTH  32
#define FUNNEL_BBOX_HEIGHT 48

#define FUNNEL_SMALL_BBOX_WIDTH	32
#define FUNNEL_SMALL_BBOX_HEIGHT 32



#define FUNNEL_TYPE_SHORT 0
#define FUNNEL_TYPE_LONG 100


class CFunnel : public CGameObject
{
protected:
	int funnelType;
public:
	CFunnel(float x, float y, int funnelType) : CGameObject(x, y) {
		type = OBJECT_TYPE_FUNNEL;
		this->funnelType = funnelType;
	}
	int GetFunnelType() { return funnelType; };
	void SetFunnelType(int funnelType) { this->funnelType = funnelType; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
