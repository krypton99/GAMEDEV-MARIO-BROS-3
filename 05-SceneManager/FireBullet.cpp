#include "FireBullet.h"
#include "AssetIDs.h"

CFireBullet::CFireBullet(float x, float y, int nx, int attackDirection) :CBullet(x, y, nx)
{
	this->direction = attackDirection;
	this->type = OBJECT_TYPE_FIREBALL;
	this->nx = nx;

}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	SetSpeed();
	x += vx*dt;
	y += vy*dt;


	
}
void CFireBullet::SetSpeed() {
	vx = nx * BULLET_SPEED_X * 0.5;

	switch (direction) {

	case RIGHT_UP_FAR:		// 8
	case LEFT_UP_FAR:		// 3
		vx = nx * BULLET_SPEED_X;
		vy = -abs(vx);
		break;

	case LEFT_UP_NEAR:		//4
	case RIGHT_UP_NEAR:		//7
		vx = nx * BULLET_SPEED_X * 0.5;
		vy = -abs(vx / 3);
		break;

	case RIGHT_DOWN_FAR:	//6
	case LEFT_DOWN_FAR:		//1
		vx = nx * BULLET_SPEED_X;
		vy = abs(vx);
		break;

	case RIGHT_DOWN_NEAR:	//5
	case LEFT_DOWN_NEAR:	//2
		vx = nx * BULLET_SPEED_X * 0.5;
		vy = abs(vx / 3);
		break;
	}

}
