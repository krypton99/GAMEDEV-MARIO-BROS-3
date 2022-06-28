#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "Grid.h"
#include "HUD.h"
#include "Camera.h"
#include"SampleKeyEventHandler.h"
class CWorldMap :
	public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 
	LPMAP map;
	LPCAMERA cam = nullptr;
	LPHUD hud;


	vector<LPGAMEOBJECT> listObjects; // chua tat ca object except item + effect

	// DOC FILE
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);
	void LoadAssets(LPCWSTR assetFile);

public:
	CWorldMap(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

};

class CWorldMapKeyHandler : public CSampleKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	//virtual void OnKeyDown(int KeyCode);
	CWorldMapKeyHandler(CScene* s) :CSampleKeyHandler(s) {};
};

