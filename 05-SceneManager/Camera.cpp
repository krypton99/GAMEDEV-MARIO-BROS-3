#include "Camera.h"
#include "Utils.h"


Camera* Camera::__instance = nullptr;
Camera* Camera::GetInstance() {
	if (!__instance) __instance = new Camera();
	return __instance;
}

Camera::Camera() {
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
	CGame::GetInstance()->SetCamPos(0, 250);
}


void Camera::Update(DWORD dt, float playerPosX, float playerPosY, float start_x, float start_y, float end_x, float end_y, bool isFlying, bool isOnGround) {

	if (islockUpdate)
		return;

	// vi tri camera lun nam giua Mario
	position_x = playerPosX;/*- width / 2 + MARIO_BIG_BBOX_WIDTH;*/



	// Check if camera out of screen map
	if (position_x > end_x)
		position_x = end_x;
	if (position_x < start_x)
		position_x = start_x;

	// Check lock_y
	if (playerPosY < end_y && isFlying)
		islockY = false;
	if (playerPosY > end_y && isOnGround && !isFlying)
		islockY = true;

	// Update pos_cam_y
	if (!islockY) { // pos_mario in center cam  < end.y (map) -> pos_cam = pos_mario...
		if (int(playerPosY - height / 4) < end_y )
		{
			position_y = float(int(playerPosY - height / 8 + 30));
		}
	}
	else if (islockY) { // id pos_cam in end-bbox of map-> pos_cam = end_map_y
		position_y = end_y;
	}
	if (position_y > end_y)
		position_y = end_y;
	if (position_y > end_y)
		position_y = start_y;
	DebugOut(L"playerPos X ==== %f \n", playerPosX);
	DebugOut(L"playerPos y ==== %f \n", playerPosY);
	DebugOut(L"position X ==== %f \n", position_x);
	DebugOut(L"position y ==== %f \n", position_y);
	CGame::GetInstance()->SetCamPos(position_x, position_y);

}