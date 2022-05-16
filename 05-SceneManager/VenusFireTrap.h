#pragma once
#include "GameObject.h"
#include "Game.h"
#include"Animation.h"
#include"Animations.h"
#include "Timer.h"

#define VENUS_MOVING_SPEED 0.005f

#define HIDDEN_TIME 2500
#define ATTACK_TIME 2500
#define VENUS_BBOX_WIDTH 16
#define VENUS_BBOX_HEIGHT 32

#define VENUS_STATE_UP 100
#define VENUS_STATE_DOWN 200
#define VENUS_STATE_FIRE 300
#define VENUS_STATE_HIDDEN 400
#define VENUS_STATE_ATTACK 500

#define ID_ANI_REDVENUS_RIGHT_DOWN 6000
#define ID_ANI_REDVENUS_RIGHT_UP 6001
#define ID_ANI_REDVENUS_LEFT_DOWN 6002
#define ID_ANI_REDVENUS_LEFT_UP 6003
#define ID_ANI_REDVENUS_MOVE 6004

#define ID_ANI_GREENVENUS_RIGHT_DOWN 6010
#define ID_ANI_GREENVENUS_RIGHT_UP 6011
#define ID_ANI_GREENVENUS_LEFT_DOWN 6012
#define ID_ANI_GREENVENUS_LEFT_UP 6013
#define ID_ANI_GREENVENUS_MOVE 6014


#define PLANT_TYPE_RED_VENUS 10
#define PLANT_TYPE_GREEN_VENUS 20

#define MARIO_LEFT_DOWN_FAR		1
#define MARIO_LEFT_DOWN_NEAR	2
#define MARIO_LEFT_UP_FAR		3
#define MARIO_LEFT_UP_NEAR		4

#define MARIO_RIGHT_DOWN_FAR	5
#define MARIO_RIGHT_DOWN_NEAR	6
#define MARIO_RIGHT_UP_FAR		7
#define MARIO_RIGHT_UP_NEAR		8
class CVenusFireTrap : public CGameObject
{
protected:
	float ax;
	float ay;
	Timer* hiddenTimer;
	Timer* attackTimer;
	ULONGLONG die_start;
	int plant_type;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	virtual void Render();
	int mario_direction = 0;
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	virtual void OnNoCollision(DWORD dt);
	bool CheckDistancePlayer(D3DXVECTOR4 player);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player);
	CVenusFireTrap(float x, float y, int plant_type);
	virtual void SetState(int state);
};