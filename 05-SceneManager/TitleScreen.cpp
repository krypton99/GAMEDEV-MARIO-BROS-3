#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "TitleScreen.h"
#include "Scene.h"
#include "SelectArrow.h"
using namespace std;

CTitleScreen::CTitleScreen(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	arrow = NULL;
	map = NULL;
	key_handler = new CTitleScreenKeyHandler(this);
}
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAPS	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2
#define ASSETS_SECTION_ANIMATION_SETS 3
#define MAX_SCENE_LINE 1024

void CTitleScreen::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void CTitleScreen::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CTitleScreen::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CTitleScreen::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	
	
	
	case OBJECT_TYPE_SELECTARROW:
	{
		int scene_id = atoi(tokens[3].c_str());
		
		obj = new CSelectArrow(x,y,scene_id);
		arrow = (CSelectArrow*)obj;
		// General object setup
		//obj->SetPosition(x, y);

		/*listObjects.push_back(obj);
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);*/
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int hasPortal = atoi(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		int left = atoi(tokens[7].c_str());
		int right = atoi(tokens[8].c_str());
		int up = atoi(tokens[9].c_str());
		int down = atoi(tokens[10].c_str());
		obj = new CPortal(hasPortal, x, y, r, b, scene_id, left, right, up, down);
		// General object setup
		//obj->SetPosition(x, y);

		/*listObjects.push_back(obj);
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);*/
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	listObjects.push_back(obj);

}


void CTitleScreen::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());

	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap);
}

void CTitleScreen::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line == "[ANIMATION_SETS]") { section = ASSETS_SECTION_ANIMATION_SETS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CTitleScreen::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[MAPS]") { section = SCENE_SECTION_MAPS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAPS: _ParseSection_MAPS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
	CGame::GetInstance()->SetCamPos(0, 0);
}

void CTitleScreen::Update(DWORD dt)
{
	

	for (size_t i = 0; i < listObjects.size(); i++)
	{
		listObjects[i]->Update(dt, &listObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	

}
void CTitleScreen::Render()
{
	map->Render();
	// list objects
	for (size_t i = 0; i < listObjects.size(); i++)
		listObjects[i]->Render();
}


void CTitleScreenKeyHandler::OnKeyDown(int KeyCode)
{
	CSelectArrow* arrow = (CSelectArrow*)((LPTITLESCENE)CGame::GetInstance()->GetCurrentScene())->GetArrow();
	CGame* game = CGame::GetInstance();
	float x, y;
	arrow->GetPosition(x, y);
	switch (KeyCode)
	{
	case DIK_X:
		CGame::GetInstance()->InitiateSwitchScene(arrow->GetSceneId());
		break;
	
	case DIK_UP:
		if (arrow->GetCanUp())
		{
			arrow->SetPosition(x, y - STEP_DISTANCE);
			arrow->SetCanDown(true);
			arrow->SetCanUp(false);
		}
		break;
	case DIK_DOWN:
		
		if (arrow->GetCanDown())
		{
			arrow->SetPosition(x, y + STEP_DISTANCE);
			arrow->SetCanDown(false);
			arrow->SetCanUp(true);
		}
		break;
	}
	
}
void CTitleScreen::Unload()
{
	for (size_t i = 0; i < listObjects.size(); i++)
		delete listObjects[i];

	listObjects.clear();
	
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}