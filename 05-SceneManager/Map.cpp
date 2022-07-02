#include "Map.h"
#include "Utils.h"
#include <string> 
#include <fstream>
#include "Textures.h"
#include "Game.h"
#include "debug.h"
#include "Camera.h"

// ANIMATION 
CMap::CMap(int mapID, LPCWSTR matrix_path, int widthMap, int heightMap) {
	this->mapID = mapID;
	this->matrix_path = matrix_path;
	this->widthMap = widthMap;
	this->heightMap = heightMap;



	getNumRow();
	getNumCol();
	LoadMap();
}


int CMap::getMapID() {
	return mapID;
}
int CMap::getWidthMap() {
	return widthMap;
}
int CMap::getHeighthMap() {
	return heightMap;
}

LPCWSTR CMap::getMatrixPath() {
	return matrix_path;
}

int CMap::getNumCol() {
	this->numCol = widthMap / TILE_SIZE;
	return numCol;
}
int CMap::getNumRow() {
	this->numRow = heightMap / TILE_SIZE;
	return numRow;
}

void CMap::LoadMap() {

	CSprites* sprites = CSprites::GetInstance();

	ifstream f;
	f.open(matrix_path, ios::in);
	if (f.fail()) {
		f.close();
		return;
	}
	//read info from file
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++) {
			f >> tileMaps[i][j];
			int a = tileMaps[i][j];
			sprites->Get(tileMaps[i][j]);
		}
	}
	f.close();


}


void CMap::Render()
{
	if (CGame::GetInstance()->GetCamPosX() == NULL)
	{
		CGame::GetInstance()->SetCamPosX(0);
	}
	int col_begin = int(CGame::GetInstance()->GetCamPosX() / TILE_SIZE);
	int col_end = int(ceil(CGame::GetInstance()->GetCamPosX() / TILE_SIZE) + (SCREEN_WIDTH / TILE_SIZE)-1); //fixing map was't render fast enough 
	/*int col_end = widthMap / TILE_SIZE;*/

	CGame* game = CGame::GetInstance();
	

	for (int i = 0; i < numRow; i++) {
		for (int j = col_begin; j < col_end; j++) {
			float x = float(TILE_SIZE * (j - col_begin) - (int)game->GetCamPosX() % TILE_SIZE + game->GetCamPosX() + CAMERA_X_OFFSET);
			float y = float(TILE_SIZE * i);


			CSprites::GetInstance()->Get(tileMaps[i][j])->Draw(x, y);

		}
	}
}

// =========== CMaps == CAnimations = Store all map ================ 

CMaps* CMaps::__instance = NULL;

CMaps* CMaps::GetInstance()
{
	if (__instance == NULL) __instance = new CMaps();
	return __instance;
}

void CMaps::Add(int id, LPMAP map)
{
	maps[id] = map;
}

LPMAP CMaps::Get(int id)
{
	LPMAP map = maps[id];
	if (map == NULL)
		return map;
	return NULL;
}