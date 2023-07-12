#include <iostream>
#include <fstream>
#include <cctype>

#include "AssetIDs.h"
#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "QuestionBrick.h"
#include "Platform.h"
#include "ColorPlatform.h"
#include "Tube.h"
#include "FireBullet.h"
#include "Card.h"
#include "Hud.h"
#include "BlackScreen.h"
#include "WorldPlayScene.h"

#include "SampleKeyEventHandler.h"
#include "GameMap.h"
#include "Camera.h"
#include "MarioWorld.h"
#include "DataBinding.h"

using namespace std;

int CPlayScene::timer = 300;
//BOOLEAN CPlayScene::init = 0;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	timer_start = GetTickCount64();
	IsWait = false;
	init = 0;
	prevHP = 4;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3
#define SCENE_SECTION_UNDERWORLD 4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines (include space line)

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
	DebugOut(L"[INFO] Load Sprites Succcess! ID: %d\n", ID);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	//1st of all, it will come here
	//Thêm 1 biến đánh dấu đã chui xuống lòng đất để khỏi phải khởi tạo lại map
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_MAP_MATRIX(string line, CMap*& map_para)
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
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
	DebugOut(L"[INFO] Load Animation Succcess! ID: %d\n", ani_id);
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

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}

	case OBJECT_TYPE_GOOMBAS: 
	{
		int Goombas_type = atoi(tokens[3].c_str());
		obj = new CGoomba(x, y, Goombas_type);

		break;
	}

	case OBJECT_TYPE_BRICK: 
	{
		int Brick_type = atoi(tokens[3].c_str());
		int Item_type = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, Brick_type, Item_type);

		break;
	}

	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int QuesBrick_type = atoi(tokens[3].c_str());
		obj = new CQuestionBrick(x, y, QuesBrick_type);

		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;

	case OBJECT_TYPE_KOOPAS: 
	{
		int Koopas_type = atoi(tokens[3].c_str());
		obj = new CKoopa(x, y, Koopas_type);

		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int isUdw_Tube = atoi(tokens[9].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, isUdw_Tube
		);

		break;
	}

	case OBJECT_TYPE_COLOR_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CColorPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_TUBE:
	{
		int Tube_type = atoi(tokens[3].c_str());
		obj = new CTube(x, y, Tube_type);

		break;
	}

	case OBJECT_TYPE_CARD:
		obj = new CCard(x, y);
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
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
		//maybe debug here
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

void CPlayScene::Load()
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
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; };
		if (line == "[UNDERWORLD]") { section = SCENE_SECTION_UNDERWORLD; continue; }

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP:	
			_ParseSection_MAP_MATRIX(line, this->map); 
			break;
		case SCENE_SECTION_UNDERWORLD: _ParseSection_MAP_MATRIX(line, this->underworld_map); 
			break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	//Problem here: if Draw Mario later it will affect the collision proccess
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way!
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//Điều chỉnh lại gravity
	HandleTimerAndWait(); //Xử lý thời gian và Wait trong game

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

	//Update Each Instance Of The Game Start From Here:
	if (!init)
		Reload(mario, player);

	CCamera::GetInstance()->Update();
	CHud::GetInstance()->Update();
	CBlackScreen::GetInstance()->Update();

	//block player if go over Min, Max of the Map
	//UNLESS When End Game
	if (mario->GetX() <= 0)
		mario->SetPosition(0, mario->GetY());
	else if (mario->GetX() >= MAP1_1_WIDTH - 10.0f && !mario->GetIsEndGame())
		mario->SetPosition(MAP1_1_WIDTH - 10.0f, player->GetY());

	//DebugOutTitle(L"X, Y, x, y: %f, %f, %f, %f", Cam->GetCamPos().x, Cam->GetCamPos().y, player->GetX(), player->GetY());
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	//Trước khi vẽ, hãy thử sắp xếp lại thứ tự vector object
	//std::sort(objects.begin(), objects.end(), );
	//CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	//Bug: 
	//1. xthrow.cpp => lỗi của Cam trong quá trình render Map (thường là sai vị trí Cam)
	//2. badAllocate => lỗi đọc Map || đọc thông tin (bị sai hoặc gì đó)
	if (mario->GetIsAtMainWorld())
	{
		if (map != NULL)
			map->Render();
		else
			DebugOut(L"[INFO] Map was NULL\n");
	}

	if (!mario->GetIsAtMainWorld() && !mario->GetIsAtWorld())
	{
		//Vẽ Underworld <=> đã reachTransPos và đc Tele xuống Underworld cũng như hết thời gian chuyển cảnh
		if (underworld_map != NULL)
			underworld_map->Render();
		else
			DebugOut(L"[INFO] UnderworldMap was NULL\n");
	}

	for (int i = 0; i < objects.size(); i++)
		if (CCamera::GetInstance()->isViewable(objects[i])) //Giảm đc 5MB
			objects[i]->Render();

	//Vẽ sau cùng tránh bị object đè: 
	//Thứ tự: Black Screen đè Hud, Hud đè Object, Object đè map
	CHud::GetInstance()->Render();
	CBlackScreen::GetInstance()->Render();
	//DebugOut(L"[INFO] Start Drawing BlackScreen\n");
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
	delete map;
	map = nullptr;
	delete underworld_map;
	underworld_map = nullptr; //Neccessary?
	init = 0; //mò cả buổi, quên mất chỉ cần đặt ở đây là thành công @@
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void CPlayScene::Reload(CMario* mario, LPGAMEOBJECT obj)
{
	CCamera::GetInstance()->SetTargetToFollow(obj);
	mario->SetIsAtWorld(0); //Xem lại sự cần thiết của IsAtWorld và AtMainWorld ??
	mario->SetHasCollectCard(0);
	CHud::GetInstance()->SetInitCard(0);
	timer = 300;
	init = 1;
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

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::AddObjectToScene(LPGAMEOBJECT game_object)
{
	this->objects.push_back(game_object);
	//Thêm vật thể vào scene hiện tại
	//objects.insert(objects.begin() + 1, game_object);
}

void CPlayScene::HandleTimerAndWait()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CMarioWorld* mario_world = (CMarioWorld*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return; //chết thì 0 đụng đến giờ nữa

	//Reset Timer if DIE || Finish Scene
	if (CDataBindings::GetInstance()->HP == prevHP - 1)
	{
		timer = 300;
		prevHP = CDataBindings::GetInstance()->HP;
		init = 0;
	}

	if (IsWait)
	{
		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->SetWait(1);
		return;
	}
	else
		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->SetWait(0);

	if (!mario->GetIsReachEndPos())
	{
		//ngưng thời gian khi ăn card (end game) hoặc khi Travelling
		if (mario->GetIsEndGame() || mario->GetIsTravelling());
		else if (GetTickCount64() - timer_start >= 1000)
		{
			if (timer == 0) return;
			timer--;
			timer_start = GetTickCount64();
		}
	}
	else //Reach End Pos thì reset thời gian nhanh chóng về 0
	{
		if (GetTickCount64() - timer_start >= 1)
		{
			if (timer == 0)
			{
				//init = 0;
				return;	//timer == 0 ngưng Update
			}
			timer--;
			timer_start = GetTickCount64();
		}
	}
	//DebugOutTitle(L"Time: %d", timer);
}