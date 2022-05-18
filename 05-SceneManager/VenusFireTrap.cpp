#include "VenusFireTrap.h"
#include "Utils.h"
#include "GameObject.h"
#include "debug.h"
#include "AssetIDs.h"
#include "Collision.h"
#include "FireBullet.h"
#define MAX_ZONE	48
CVenusFireTrap::CVenusFireTrap(float x, float y, int plant_type) :CGameObject(x, y)
{
	this->ax = 0;
	hiddenTimer = new Timer(HIDDEN_TIME);
	attackTimer = new Timer(ATTACK_TIME);
	GetPosition(this->start_x, this->start_y);
	SetState(VENUS_STATE_UP);
	type = OBJECT_TYPE_VENUS;
	this->plant_type = plant_type;
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - VENUS_BBOX_WIDTH / 2;
	top = y - VENUS_BBOX_HEIGHT / 2;
	right = left + VENUS_BBOX_WIDTH;
	bottom = top + VENUS_BBOX_HEIGHT;
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CVenusFireTrap*>(e->obj)) return;


}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player)
{
	//CGameObject::Update(dt);
	this->y += vy * dt;
	vy += ay * dt;
	vx += ax * dt;
	if (GetState() == VENUS_STATE_UP && y < start_y - VENUS_BBOX_HEIGHT) {
		y = start_y - VENUS_BBOX_HEIGHT; // vi tri y tang len maximum ~ flower height
		SetState(VENUS_STATE_ATTACK);
	}
	if (attackTimer->GetStartTime() != 0 && attackTimer->IsTimeUp()) {
		attackTimer->Stop();
		SetState(VENUS_STATE_DOWN);
	}
	// step 3: (move down)  -> het attackTimer 
	if (GetState() == VENUS_STATE_DOWN && y > start_y) {
		y = start_y + 1;
		SetState(VENUS_STATE_HIDDEN);
	}

	// step 4: (hidden)  -> het hiddenTimer -> moving up (step 1) 
	if (hiddenTimer->GetStartTime() != 0 && hiddenTimer->IsTimeUp()) {
		hiddenTimer->Stop();
		SetState(VENUS_STATE_UP);
	}

	if (player.x < start_x) {
		// mario_x o ben TRAI fireflower
		if (player.y > this->y) {
			// mario_y THAP hon fireflower
			if (CheckDistancePlayer(player))
				mario_direction = MARIO_LEFT_DOWN_NEAR;
			else
				mario_direction = MARIO_LEFT_DOWN_FAR;
		}
		else {
			// mario_y CAO hon fireflower
			if (CheckDistancePlayer(player))
				mario_direction = MARIO_LEFT_UP_NEAR;
			else
				mario_direction = MARIO_LEFT_UP_FAR;
		}
		nx = -1;
	}
	else {
		// mario_x o ben PHAI fireflower
		if (player.y > this->y) {
			// mario_y THAP hon fireflower
			if (CheckDistancePlayer(player))
				mario_direction = MARIO_RIGHT_DOWN_NEAR;
			else
				mario_direction = MARIO_RIGHT_DOWN_FAR;
		}
		else {
			// mario_y CAO hon fireflower
			if (CheckDistancePlayer(player))
				mario_direction = MARIO_RIGHT_UP_NEAR;
			else
				mario_direction = MARIO_RIGHT_UP_FAR;
		}
		nx = 1;
	}
	if (isAttack) {
		if (nx > 0) { // PHAI
			fireBullet = new CFireBullet( x , y  , nx, mario_direction);
		}
		else {
			fireBullet = new CFireBullet( x , y  , nx, mario_direction);
		}
		isAttack = false;
		coObjects->push_back(fireBullet);
	}

	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CVenusFireTrap::Render()
{
	int ani = ID_ANI_REDVENUS_MOVE;
	
	switch (mario_direction)
	{
	case MARIO_LEFT_DOWN_FAR:
	case MARIO_LEFT_DOWN_NEAR:
		ani = ID_ANI_REDVENUS_LEFT_DOWN;
		break;
	case MARIO_LEFT_UP_FAR:
	case MARIO_LEFT_UP_NEAR:
		ani = ID_ANI_REDVENUS_LEFT_UP;
		break;
	case MARIO_RIGHT_DOWN_FAR:
	case MARIO_RIGHT_DOWN_NEAR:
		ani = ID_ANI_REDVENUS_RIGHT_DOWN;
		break;
	case MARIO_RIGHT_UP_FAR:
	case MARIO_RIGHT_UP_NEAR:
		ani = ID_ANI_REDVENUS_RIGHT_UP;
		break;
	}
	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	
	//RenderBoundingBox();
}
bool CVenusFireTrap::CheckDistancePlayer(D3DXVECTOR4 player)
{
	if (abs(player.x - start_x) < MAX_ZONE) // trong vung gan hoa
		return true;
	return false;
}

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case VENUS_STATE_ATTACK:
		vy = 0;
		isAttack = true;
		attackTimer->Start();
		break;
	case VENUS_STATE_HIDDEN:
		vy = 0;
		hiddenTimer->Start();
		break;
	case VENUS_STATE_UP:
		vy = -VENUS_MOVING_SPEED;
		break;
	case VENUS_STATE_DOWN:
		vy = VENUS_MOVING_SPEED;
		break;
	}
}
