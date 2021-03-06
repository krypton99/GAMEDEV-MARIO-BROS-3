#pragma once

#include "GameObject.h"
#include "Ghost.h"
#include "AssetIDs.h"
#define PLATFORM_TYPE_BLOCK 0
#define PLATFORM_TYPE_GROUND 1
#define PLATFORM_TYPE_DEATHZONE 2
// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int filterX = 0;
	int filterY = 0;
	int platform_type;
public: 
	int GetPlatformType() { return this->platform_type; };
	void SetPlatformType(int platform_type) { this->platform_type = platform_type; };
	CGhostPlatform* ghost = NULL;
	int isBlockingX=-1;
	int IsBlocking() {
		if (platform_type != PLATFORM_TYPE_DEATHZONE) { return 1; }
		else { return 0;}
	}
	int IsBlockingX() { return isBlockingX; }
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,int type) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->type = OBJECT_TYPE_PLATFORM;
		this->platform_type = type;
	}
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	
};

typedef CPlatform* LPPLATFORM;