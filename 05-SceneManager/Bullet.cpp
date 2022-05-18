#include "Bullet.h"
#include "AssetIDs.h"

CBullet::CBullet(float x, float y, int nx) :CGameObject(x, y)
{
	this->x = x;
	this->y = y;
	this->nx = nx;
	
}
void CBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int ani = 0;
	animations->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BULLET_BBOX_WIDTH / 2;
	t = y - BULLET_BBOX_HEIGHT / 2;
	r = l + BULLET_BBOX_WIDTH;
	b = t + BULLET_BBOX_HEIGHT;
}
void CBullet::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//CGameObject::Update(dt);
	this->vy += BULLET_GRAVITY * dt;	// Simple fall down
	this->vx = nx * BULLET_SPEED_X;

	
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{


}
