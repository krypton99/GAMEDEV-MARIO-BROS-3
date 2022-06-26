#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Map.h"
#include"SampleKeyEventHandler.h"
class CTitleScreen :
	public CScene
{
protected:
	LPMAP map;
	LPCAMERA cam = nullptr;


	vector<LPGAMEOBJECT> listObjects; // chua tat ca object except item + effect

	// DOC FILE
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);
	void LoadAssets(LPCWSTR assetFile);

public:
	CTitleScreen(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

};

class CTitleScreenKeyHandler : public CSampleKeyHandler
{
public:
	//virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	CTitleScreenKeyHandler(CScene* s) :CSampleKeyHandler(s) {};
};

