#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopas.h"
#include "debug.h"
#include "MarioTail.h"
#include "Funnel.h"
#include "Portal.h"
#define MARIO_TIME_ATTACK 400
#define TIME_FLY					2000
#define TIME_POWER_UP	2000
#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f
#define MARIO_RACOON_FALLING_SLOW_SPEED -0.08f
#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f
#define MARIO_ACCEL_STOP_X 0.0004f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_WALKING_UP_WORLD_MAP		150 //mario in world map
#define MARIO_STATE_WALKING_DOWN_WORLD_MAP		151 //mario in world map
#define MARIO_STATE_WALKING_RIGHT_WORLD_MAP		152 //mario in world map
#define MARIO_STATE_WALKING_LEFT_WORLD_MAP		153 //mario in world map
#define MARIO_STATE_IDLE_WORLD_MAP	350

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY				700
#define MARIO_STATE_RELEASE_FLY     800

#define MARIO_STATE_ATTACK	900

#define MARIO_STATE_POWER_UP	990

#define MARIO_STATE_TELEPORT	980

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1103
#define ID_ANI_MARIO_BRACE_LEFT 1104

#define ID_ANI_MARIO_IDLE_HOLDING_RIGHT_RED 1110
#define ID_ANI_MARIO_IDLE_HOLDING_LEFT_RED 1111
#define ID_ANI_MARIO_IDLE_HOLDING_RIGHT_GREEN 1112
#define ID_ANI_MARIO_IDLE_HOLDING_LEFT_GREEN 1113

#define ID_ANI_MARIO_WALK_HOLDING_RIGHT_RED 1121
#define ID_ANI_MARIO_WALK_HOLDING_LEFT_RED 1122
#define ID_ANI_MARIO_WALK_HOLDING_RIGHT_GREEN 1123
#define ID_ANI_MARIO_WALK_HOLDING_LEFT_GREEN 1124
#define ID_ANI_MARIO_PIPE 1125
#define ID_ANI_MARIO_DIE 999
#define ID_ANI_MARIO_BIG_WORLDMAP			2501


// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_IDLE_HOLDING_RIGHT_RED 1610
#define ID_ANI_MARIO_SMALL_IDLE_HOLDING_LEFT_RED 1611
#define ID_ANI_MARIO_SMALL_IDLE_HOLDING_RIGHT_GREEN 1612
#define ID_ANI_MARIO_SMALL_IDLE_HOLDING_LEFT_GREEN 1613

#define ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_RED 1620
#define ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_RED 1621
#define ID_ANI_MARIO_SMALL_WALK_HOLDING_RIGHT_GREEN 1622
#define ID_ANI_MARIO_SMALL_WALK_HOLDING_LEFT_GREEN 1623
#define ID_ANI_MARIO_SMALL_PIPE 1624
#define ID_ANI_MARIO_SMALL_WORLDMAP			2500

// RACOON MARIO

#define ID_ANI_RACOON_MARIO_IDLE_RIGHT 1701
#define ID_ANI_RACOON_MARIO_IDLE_LEFT 1700

#define ID_ANI_RACOON_MARIO_WALKING_RIGHT 1801
#define ID_ANI_RACOON_MARIO_WALKING_LEFT 1800

#define ID_ANI_RACOON_MARIO_RUNNING_RIGHT 2001
#define ID_ANI_RACOON_MARIO_RUNNING_LEFT 2000

#define ID_ANI_RACOON_MARIO_JUMP_WALK_RIGHT 1901
#define ID_ANI_RACOON_MARIO_JUMP_WALK_LEFT 1900

#define ID_ANI_RACOON_MARIO_FLY_RIGHT 2401
#define ID_ANI_RACOON_MARIO_FLY_LEFT 2400

#define ID_ANI_RACOON_MARIO_SIT_RIGHT 2201
#define ID_ANI_RACOON_MARIO_SIT_LEFT 2200

#define ID_ANI_RACOON_MARIO_BRACE_RIGHT 2101
#define ID_ANI_RACOON_MARIO_BRACE_LEFT 2100

#define ID_ANI_MARIO_RACOON_IDLE_HOLDING_RIGHT_RED 2410
#define ID_ANI_MARIO_RACOON_IDLE_HOLDING_LEFT_RED 2411
#define ID_ANI_MARIO_RACOON_IDLE_HOLDING_RIGHT_GREEN 2412
#define ID_ANI_MARIO_RACOON_IDLE_HOLDING_LEFT_GREEN 2413

#define ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_RED 2420
#define ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_RED 2421
#define ID_ANI_MARIO_RACOON_WALK_HOLDING_RIGHT_GREEN 2422
#define ID_ANI_MARIO_RACOON_WALK_HOLDING_LEFT_GREEN 2423
#define ID_ANI_MARIO_RACOON_PIPE 2424
#define ID_ANI_MARIO_RACOON_FALL_SLOW_LEFT 2425
#define ID_ANI_MARIO_RACOON_FALL_SLOW_RIGHT 2426

#define ID_ANI_RACOON_MARIO_ATTACK_RIGHT	2301
#define ID_ANI_RACOON_MARIO_ATTACK_LEFT		2300
#define ID_ANI_MARIO_RACOON_WORLDMAP		2502
#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACOON_BBOX_WIDTH			15
#define MARIO_RACOON_BBOX_HEIGHT			28
#define MARIO_RACOON_SITTING_BBOX_WIDTH  15
#define MARIO_RACOON_SITTING_BBOX_HEIGHT 18


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	float score;
	int stage = NULL;
	BOOLEAN isSitting;
	float maxVx;
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	bool isHolding = false;
	int level; 
	bool isPowerUp;
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 
	bool isFlying = false;
	bool mLeft;	// mario in worldmap
	bool mRight;
	bool mUp;
	bool mDown;
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithGhostKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithVenus(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithPswitch(LPCOLLISIONEVENT e);
	void OnCollisionWithFunnel(LPCOLLISIONEVENT e);
	void OnCollisionWithEndSceneItems(LPCOLLISIONEVENT e);
	float desX;
	float desY;
	float isInPipe;
	float isOutPipe;
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();
	void DecreaseSpeed();
	bool isFallLower = false;
	CKoopas* shell = nullptr;
	int changeNx = 0;
	bool isHitY = false;
	CMarioTail* tail = CMarioTail::GetInstance(x, y);
	CGhost* ghost_mario = nullptr;
	boolean isBlocking = true;
	int temp_nx;
	bool isAttack;
	bool isInHiddenMap=false;
	CGhostPlatform* collide_Platform = nullptr;
	bool canGoThroughPipe_down;
	bool canGoThroughPipe_up;
	bool canSwitchScene = false;
	Timer* getInPipe=new Timer(2000);
	CFunnel* funnel = nullptr;
	bool isFlystep;
	vector<int> cards;
	int lastnx;
	CPortal* collidePortal;
	
public:
	bool canHolding;
	void setAy(float ay) { this->ay = ay; }
	void setAx(float ax) { this->ax = ax; }
	CMario(float x, float y) : CGameObject(x, y)
	{
		score = 0;
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		tail = new CMarioTail(x, y);
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		getInPipe = new Timer(2000);
	}
	CMario(float x, float y, int stage) : CGameObject(x, y)
	{
		this->stage = stage;
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		if (stage == WORLD_MAP_SCENE) {
			ay = 0;
		}
		else ay = MARIO_GRAVITY;
		tail = new CMarioTail(x, y);
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		getInPipe = new Timer(2000);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateWorldMap(DWORD dt, vector<LPGAMEOBJECT>* coObjects); //update mario khi o worldmap
	void Render();
	void SetState(int state);
	CPortal* GetCollidePortal() { return collidePortal; }
	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE && isBlocking);
	}
	
	//float platform_PosX(CGhostPlatform* platform);
	Timer* flyTime = new Timer(TIME_FLY);
	Timer* flyStep = new Timer(500);
	Timer* attackStart = new Timer(MARIO_TIME_ATTACK);
	Timer* PowerUp = new Timer(TIME_POWER_UP);
	int IsBlocking() { return 0; }
	int IsBlockingY() {return 1;}
	void SetStage(int stage) { this->stage = stage; }
	int GetStage() { return this->stage; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetLevel() { return level; };
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	bool GetIsHolding() { return isHolding; }
	void SetIsHolding(bool isHolding) { this->isHolding = isHolding; }
	bool GetIsInPipe() { return isInPipe; }
	void SetIsInPipe(bool isInPipe) { this->isInPipe = isInPipe; }
	bool GetIsOutPipe() { return isOutPipe; }
	void SetIsOutPipe(bool isOutPipe) { this->isOutPipe = isOutPipe; }
	bool GetIsCanGoThroughPipeDown() { return canGoThroughPipe_down; }
	void SetIsCanGoThroughPipeDown(bool canGoThroughPipe_down) { this->canGoThroughPipe_down = canGoThroughPipe_down; }
	bool GetIsCanGoThroughPipeUp() { return canGoThroughPipe_up; }
	void SetIsCanGoThroughPipeUp(bool canGoThroughPipe_up) { this->canGoThroughPipe_up = canGoThroughPipe_up; }
	bool GetIsPowerUp() { return isPowerUp; }
	void SetIsPowerUp(bool isPowerUp) { this->isPowerUp = isPowerUp; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool GetIsFlying() { return isFlying; }
	void SetIsFlying(bool isFlying) { this->isFlying = isFlying; }
	bool GetIsOnPlatform() { return isOnPlatform; }
	void SetIsOnPlatform(bool isOnPlatform) { this->isOnPlatform = isOnPlatform; }
	bool GetFallLower() { return isFallLower; }
	void SetFallLower(bool isFallLower) { this->isFallLower = isFallLower; }
	float GetVy() { return vy; }
	void SetVy(float vy) { this->vy = vy; }
	CMarioTail* GetTail() { return tail; }
	void SetTail(CMarioTail* tail) { this->tail = tail; }
	CKoopas* GetShell() { return shell; }
	void SetShell(CKoopas* shell) { this->shell = shell; }
	bool GetIsAttack() { return isAttack; }
	void SetIsAttack(bool isAttack) { this->isAttack = isAttack; }
	bool GetIsFlystep() { return isFlystep; }
	void SetIsFlystep(bool IsFlystep) { this->isFlystep = IsFlystep; }
	bool GetIsBlocking() { return isBlocking; }
	void SetIsBlocking(bool isBlocking) { this->isBlocking = isBlocking; }
	bool GetIsInHiddenMap() { return isInHiddenMap; }
	void SetIsInHiddenMap(bool isInHiddenMap) { this->isInHiddenMap = isInHiddenMap; }
	CGhostPlatform* GetCollidePlatform() { return collide_Platform; }
	void SetCollidePlatform(CGhostPlatform* collide_Platform) { this->collide_Platform = collide_Platform; }
	CGhost* GetGhostMario() { return ghost_mario; }
	void SetGhostMario(CGhost* ghost_mario) { this->ghost_mario = ghost_mario; }
	void SetCanSwitchScene(BOOLEAN canSwitchScene) { this->canSwitchScene = canSwitchScene; }
	bool GetCanSwitchScene() { return this->canSwitchScene; }
	bool getMLeft() { return this->mLeft; }
	bool getMRight() { return this->mRight; }
	bool getMUp() { return this->mUp; }
	bool getMDown() { return this->mDown; }
	void setMRight(bool mRight) { this->mRight = mRight; }
	float getScore() { return this->score; }
	int getCoin() { return this->coin; }
	void AddScore(int score) { this->score += score; }
	vector<int> GetCards() { return cards; }
};