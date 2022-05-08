#include "Ghost.h"
#include "Textures.h"



void CGhost::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CGhost::Render()
{
	RenderBoundingBox();
}

void CGhost::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}


CGhostKoopas::CGhostKoopas(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GHOST_GRAVITY;
	die_start = -1;
	SetState(GHOST_STATE_STICK);
	start_x = x;
	start_y = y;
}

void CGhostKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	{
		left = x - GHOST_KOOPAS_BBOX_WIDTH / 2;
		top = y - GHOST_KOOPAS_BBOX_HEIGHT / 2;
		right = left + GHOST_KOOPAS_BBOX_WIDTH;
		bottom = top + GHOST_KOOPAS_BBOX_HEIGHT;
	}
}

void CGhostKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGhostKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGhost*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) // va cham ground
		{
			isOnGround = true;

		}
		
	}
	else if (e->nx != 0)
	{
		vx = -vx;

	}

}

void CGhostKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;
	vx += ax * dt;
	//CGameObject::Update(dt, coObjects);
	isOnGround = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGhostKoopas::Render()
{
	RenderBoundingBox();
}

void CGhostKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GHOST_STATE_STICK:
		vx = 0.05f;
		break;
	}
}
