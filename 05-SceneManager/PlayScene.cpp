#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Ghost.h"
#include "Platform.h"
#include "Mushroom.h"
#include "SampleKeyEventHandler.h"
#include "Item.h"
#include "Funnel.h"
#include "VenusFireTrap.h"
#include "Koopas.h"
#include "Leaf.h"
#include "HUD.h"
#include "Pswitch.h"
#include "CoinEffect.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	grid = NULL;
	hud = NULL;
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAPS	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	CGameObject *ghost = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		obj = new CMario(x,y); 
		player = (CMario*)obj;  
		player->GetTail()->GetInstance(x, y);
		player->SetStage(this->id);
		player->SetGhostMario(new CGhost(x, y));
		player->GetGhostMario()->SetGhostType(GHOST_TYPE_MARIO);
		objects.push_back(player->GetTail());
		objects.push_back(player->GetGhostMario());
		DebugOut(L"[INFO] Player object has been created!\n");
		hud = new HUD();
		break;
	case OBJECT_TYPE_GOOMBA: {
		float goomba_type = (float)atof(tokens[3].c_str());
		int top = atoi(tokens[4].c_str());
		int bot = atoi(tokens[5].c_str());
		int left = atoi(tokens[6].c_str());
		int right = atoi(tokens[7].c_str());
		obj = new CGoomba(x, y,goomba_type); 
		obj->SetPosition(x, y);
		listMoving.push_back(obj);
		for (int row = top; row < bot; row++) {
			for (int col = left; col < right; col++)
				grid->PushObjectsIntoGrid(obj, row, col);
		}
		break;
	}
	case OBJECT_TYPE_FUNNEL: {
		int funnelType = (int)atoi(tokens[3].c_str());
		int isEntry = (int)atoi(tokens[4].c_str());
		float desX = (float)atof(tokens[5].c_str());
		float desY = (float)atof(tokens[6].c_str());
		int direction = (int)atoi(tokens[7].c_str());
		obj = new CFunnel(x, y, funnelType,isEntry,desX,desY,direction); break;
	}
	case OBJECT_TYPE_VENUS: {
		int plant_type = (int)atoi(tokens[3].c_str());
		obj = new CVenusFireTrap(x, y, plant_type);
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		float type = (float)atof(tokens[3].c_str());
		int top = atoi(tokens[4].c_str());
		int bot = atoi(tokens[5].c_str());
		int left = atoi(tokens[6].c_str());
		int right = atoi(tokens[7].c_str());
		obj = new CKoopas(x, y, type);
		obj->SetPosition(x, y);
		listMoving.push_back(obj);
		for (int row = top; row < bot; row++) {
			for (int col = left; col < right; col++)
				grid->PushObjectsIntoGrid(obj, row, col);
		}
		break;
		
	}
	case OBJECT_TYPE_BRICK: 
	{
		float brickType = (float)atof(tokens[3].c_str());
		int itemType = (int)atoi(tokens[4].c_str());
		obj = new CBrick(x, y, brickType, itemType); break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		CPlatform* platform = NULL;
		platform = new CPlatform(
			x, y,
			cell_width, cell_height, length, type
		);
		if (type == PLATFORM_TYPE_BLOCK) {
			platform->isBlockingX = 0;
		} else platform->isBlockingX = 1;
		/*CGhostPlatform* ghost = new CGhostPlatform(x, y+16,
			cell_width+1, 16, length
		);
		platform->ghost = ghost;*/
		objects.push_back(platform);
		/*objects.push_back(ghost);*/
		break;
	}

	/*case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;*/


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	

	if (object_type != OBJECT_TYPE_PLATFORM && object_type != OBJECT_TYPE_GOOMBA && object_type != OBJECT_TYPE_KOOPAS) {
		obj->SetPosition(x, y);
		objects.push_back(obj);
	}
	cam = Camera::GetInstance();
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
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
void CPlayScene::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());
	int lockHiddenMap = atoi(tokens[4].c_str());

	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap);
	map->setLockHiddenMapWidth(lockHiddenMap);
	CMaps::GetInstance()->Add(map_id, map);
	if (map) {
		grid = new CGrid(map->getWidthMap(), map->getHeighthMap());
	}
}
void CPlayScene::GetObjectToGrid() {
	// xoa het cac object
	//listItems.clear();
	//objects.clear();
	listGrid.clear();
	
	grid->GetObjectFromGrid(listGrid, player);

	//for (UINT i = 0; i < listGrid.size(); i++) {
	//	listObject.push_back(listGrid[i]);
	//}
}
void CPlayScene::Load()
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
	CGameObject* obj = NULL;
	CPlatform* platform = NULL;
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	grid->ResetGrid(listMoving);
	GetObjectToGrid();
	vector<LPGAMEOBJECT> coObjects;
	
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i]->GetType() == OBJECT_TYPE_PSWITCH) {
			CPswitch* p_switch = dynamic_cast<CPswitch*>(objects[i]);
			p_switch->Update(dt, &objects, &listItems);
		}
		else
		if (objects[i]->GetType() == OBJECT_TYPE_BRICK) {
			CBrick* brick = dynamic_cast<CBrick*>(objects[i]);
			if (brick->GetIsFallingItem() ) {
				//CREATE ITEM FOLLOW MARIO LEVEL
				Item* item = NULL;
				CGameObject* obj = NULL;
				if (brick->GetItemType() == CONTAIN_MUSHROOM) {
					if (player->GetLevel() == MARIO_LEVEL_BIG)
						item = new CLeaf({ brick->GetPosX(), brick->GetPosY() - ITEM_BBOX * 3 , ITEM_LEAF });
					else item = new CMushroom(brick->GetPosX(), brick->GetPosY() - ITEM_BBOX, ITEM_RED_MUSHROOM);
				}
				else if (brick->GetItemType() == CONTAIN_PSWITCH) {
					obj = new CPswitch(brick->GetPosX(), brick->GetPosY());
				}
				else if (brick->GetItemType() == CONTAIN_COIN) {
					float b_x, b_y;
					player->AddScore(100);
					brick->GetPosition(b_x, b_y); // vi tri cua brick
					obj = new CCoinEffect({ b_x,b_y });
					listEffects.push_back(obj);
				}
				if (item != NULL && brick->GetItemType() != CONTAIN_COIN) {
					listItems.push_back(item);
				}
				if (obj != NULL && brick->GetItemType() != CONTAIN_COIN) {
					objects.push_back(obj);
				} /*else return;*/
				brick->SetIsFallingItem(false);
			}
		}
		
		
		if (objects[i]->GetType() == OBJECT_TYPE_VENUS) {
			float l, t, r, b;
			player->GetBoundingBox(l, t, r, b);
			CVenusFireTrap* venus = dynamic_cast<CVenusFireTrap*>(objects[i]);
			venus->Update(dt, &objects, { l,t,r,b });
		}
		}
	for (size_t i = 0; i < listGrid.size(); i++) { // update listGrid objects 
		if (listGrid[i]->GetType() == OBJECT_TYPE_KOOPAS) {
			CKoopas* koopas = dynamic_cast<CKoopas*>(listGrid[i]);

			float x, y;
			koopas->GetPosition(x, y);
			if (koopas->GetIsGhostFollow()) {
				koopas->SetGhost_koopas(new CGhost(x + 16, y));
				koopas->GetGhost_koopas()->SetGhostType(GHOST_TYPE_KOOPAS);
				if (koopas->GetGhost_koopas() != NULL) {
					objects.push_back(koopas->GetGhost_koopas());
				}
				else return;
				koopas->SetIsGhostFollow(false);
			}
		}
	}
	for (size_t i = 0; i < listEffects.size(); i++) {
		listEffects[i]->Update(dt);

		// effect score effect after hit money
		if (listEffects[i]->GetType() == OBJECT_TYPE_COIN && listEffects[i]->GetState() == STATE_ERASE) {
			float c_x, c_y;
			listEffects[i]->GetPosition(c_x, c_y);
			//CGameObject* score = new ScoreEffect({ c_x, c_y }, 100);
			//listEffects.push_back(score);
		}

	}
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < listItems.size(); i++)
	{
		coObjects.push_back(listItems[i]);
	}
	for (size_t i = 0; i < listGrid.size(); i++)
	{
		coObjects.push_back(listGrid[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < listItems.size(); i++) {
		listItems[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < listGrid.size(); i++) {
		listGrid[i]->Update(dt, &coObjects);

	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;
	if (cx < 0) cx = 0;
	cam->Update(dt, cx, cy, 0, 0, float(map->getWidthMap() - SCREEN_WIDTH), float(map->getHeighthMap()-SCREEN_HEIGHT+HUD_HEIGHT), player->GetIsFlying(), player->GetIsOnPlatform(), player->GetIsInHiddenMap(), map->getLockHiddenMapWidth());

	/*if (cx < 0) cx = 0;*/
	grid->UpdateOnGrid(listMoving);
	//CGame::GetInstance()->SetCamPos(cx, cy);
	for (size_t i = 0; i < listItems.size(); i++) {
		if (listItems[i]->GetState() == STATE_ERASE) {

			listItems.erase(listItems.begin() + i);
			i--;
		}
	}
	for (size_t i = 0; i < listGrid.size(); i++) {
		if (listGrid[i]->GetState() == STATE_ERASE) {

			listGrid.erase(listGrid.begin() + i);
			i--;
		}
	}
	for (size_t i = 0; i < listEffects.size(); i++) {
		if (listEffects[i]->GetState() == STATE_ERASE) {

			listEffects.erase(listEffects.begin() + i);
			i--;
		}
	}
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	map->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	for (int i = 0; i < listItems.size(); i++)
		listItems[i]->Render();
	for (int i = 0; i < listGrid.size(); i++)
		listGrid[i]->Render();
	for (size_t i = 0; i < listEffects.size(); i++)
		listEffects[i]->Render();
	hud->Render(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY(), player);
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	for (it = listItems.begin(); it != listItems.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
	listItems.erase(
		std::remove_if(listItems.begin(), listItems.end(), CPlayScene::IsGameObjectDeleted),
		listItems.end());
}