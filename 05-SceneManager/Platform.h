#pragma once

#include "GameObject.h"
#include "Ghost.h"
#include "AssetIDs.h"
// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int isThrough=0;
public: 
	CGhost* ghost = NULL;
	int GetIsThrough() { return this->isThrough; }
	void SetIsThrough(int isThrough) { this->isThrough = isThrough; }
	int IsBlocking() { return isThrough == 0; }
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->type = OBJECT_TYPE_PLATFORM;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;