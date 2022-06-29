#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "Camera.h"
#include "HUD.h"
#include "Grid.h"
//#include "Koopas.h"
#define PLAY_TIME			300 //in second

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;
	LPMAP map;
	LPHUD hud;
	LPCAMERA cam = nullptr;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listMoving;
	vector<LPGAMEOBJECT> listGrid;
	vector<LPGAMEOBJECT> listEffects;
	//vector<LPGAMEOBJECT> listObject;
	LPGRID grid;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_MAPS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	Timer* playTime = new Timer(PLAY_TIME * 1000);
	void LoadAssets(LPCWSTR assetFile);
	int remainingTime = 0;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	int getRemainingTime() { return remainingTime; }
	void Clear();
	void PurgeDeletedObjects();
	void GetObjectToGrid();
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

