#include <iostream>
#include <fstream>
#include <cctype>

#include "AssetIDs.h"
#include "WorldPlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "MarioWorld.h"
#include "Grass.h"
#include "Entrance.h"
#include "BlockPoint.h"
#include "GateKeeper.h"
#include "Hud.h"
#include "BlackScreen.h"

#include "WorldKeyHandler.h"
#include "GameMap.h"
#include "Camera.h"
#include "debug.h"

using namespace std;

CWorldPlayScene::CWorldPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CWorldMapKeyEvent(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CWorldPlayScene::_ParseSection_SPRITES(string line) 
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

void CWorldPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CWorldPlayScene::_ParseSection_MAP_MATRIX(string line, CMap*& map_para)
{
	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles, dr_st_y;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;
	DebugOut(L"[INFO] Start Parse Map From File: %s\n", path);

	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles >> dr_st_y;

	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++)
			f >> TileMapData[i][j]; //Đọc và tạo Ma trận map
	}
	f.close();

	map_para = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles, dr_st_y);
	map_para->ClipSpritesFromTileset(); //bóc từng sprite từ tileSet
	map_para->SetMapMatrix(TileMapData);
	DebugOut(L"[INFO] Parse Map Matrix Success: %s \n", path);
}

void CWorldPlayScene::_ParseSection_ANIMATIONS(string line) 
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CWorldPlayScene::_ParseSection_OBJECTS(string line) 
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
	case OBJECT_TYPE_MARIO_AT_WORLD_MAP:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMarioWorld(x, y);
		player = (CMarioWorld*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GRASS: 
	{
		int isHelp = (int)atoi(tokens[3].c_str());
		obj = new CGrass(x, y, isHelp);
	}
		break;

	case OBJECT_TYPE_ENTRANCE:
	{
		int type = (int)atoi(tokens[3].c_str());
		int scene_id= (int)atoi(tokens[4].c_str());

		float l = (float)atoi(tokens[5].c_str());
		float t = (float)atoi(tokens[6].c_str());
		float r = (float)atoi(tokens[7].c_str());
		float b = (float)atoi(tokens[8].c_str());

		obj = new CEntrance(x, y, type, scene_id, l, t, r, b);
		break;
	}
	case OBJECT_TYPE_BLOCK_POINT:
	{
		float l = (float)atoi(tokens[3].c_str());
		float t = (float)atoi(tokens[4].c_str());
		float r = (float)atoi(tokens[5].c_str());
		float b = (float)atoi(tokens[6].c_str());
		obj = new CBlockPoint(x, y, l, t, r, b);
		break;
	}
	case OBJECT_TYPE_GATE_KEEPER: obj = new CGateKeeper(x, y);
		break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}
	obj->SetPosition(x, y);
	objects.insert(objects.begin(), obj);
	//objects.push_back(obj);

}

void CWorldPlayScene::LoadAssets(LPCWSTR assetFile) 
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

void CWorldPlayScene::Load() 
{
	//Use for Load Scene
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
		if (line == "[MAP]") 
		{ 
			section = SCENE_SECTION_MAP; 
			continue; 
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: 
			_ParseSection_ASSETS(line); 
			break;
		case SCENE_SECTION_OBJECTS: 
			_ParseSection_OBJECTS(line);
			break;
		case SCENE_SECTION_MAP: 
			_ParseSection_MAP_MATRIX(line, this->world_map); 
			break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CWorldPlayScene::Update(DWORD dt) 
{
	//Với Hud ở World thì khởi tạo 1 lần duy nhất, 0 CẦN Update
	CBlackScreen::GetInstance()->Update();

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	PurgeDeletedObjects();
}

void CWorldPlayScene::Render() 
{
	world_map->Render();
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	CHud::GetInstance()->Render(); //Done major bug
	CBlackScreen::GetInstance()->Render(); //prob here
}

void CWorldPlayScene::Unload() 
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void CWorldPlayScene::Clear() 
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

bool CWorldPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CWorldPlayScene::PurgeDeletedObjects()
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

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CWorldPlayScene::IsGameObjectDeleted),
		objects.end());
}