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
	//vector<LPGAMEOBJECT> listObject;
	LPGRID grid;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_MAPS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	void GetObjectToGrid();
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

