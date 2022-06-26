#pragma once
#include "Utils.h"
#include "Game.h"
#include "Mario.h"
#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 240
#define CAMERA_X_OFFSET 10
class Camera
{
private:
	static Camera* __instance;
	float position_x;
	float position_y;
	bool islockUpdate = false;	// lock when mario die
	bool islockY;				// lock when mario isOnGround and jump to get higher ->cam not move
	int width;
	int height;

public:
	static Camera* GetInstance();

	Camera();
	void Update(DWORD dt, float playerPosX, float playerPosY, float start_x, float start_y, float end_x, float end_y, bool isFlying, bool isOnGround);

	void ResetPosition() { position_x = 0; position_y = 0; }

	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	bool IsLockUpdate() { return this->islockUpdate; }
	void SetLockUpdate() { this->islockUpdate = true; }
	void SetUnLockUpdate() { this->islockUpdate = false; }

};
typedef Camera * LPCAMERA;