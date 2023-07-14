#include <iostream>
#include <fstream>
#include <cctype>

#include "AssetIDs.h"
#include "PlayScene.h"
#include "IntroPlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "IntroLayer.h"
#include "RedCurtain.h"
#include "Mushroom.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Platform.h"
#include "ColorPlatform.h"
#include "DataBinding.h"
#include "RedArrow.h"
#include "MarioNPC.h"
#include "LuigiNPC.h"

#include "IntroKeyHandler.h"
#include "WorldKeyHandler.h"
#include "debug.h"

bool CIntroPlayScene::allowGoombaToMove = 0;
bool CIntroPlayScene::allowLuigiToThrowKoopa = 0;
bool CIntroPlayScene::allowReleaseMarioSit = 0;
bool CIntroPlayScene::allowMarioRun = 0;
bool CIntroPlayScene::allowLuigiToRunRight = 0;
bool CIntroPlayScene::LuigiHasRunRight = 0;
bool CIntroPlayScene::hasTurnSmall = 0;
bool CIntroPlayScene::allowUseArrow = 0;
bool CIntroPlayScene::hasUpdateArrowPos = 0;
bool CIntroPlayScene::LuigiHasThrowKoopa = 0;

using namespace std;

CIntroPlayScene::CIntroPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	//Intro 0 có Player ?, toàn NPC
	Priority = 0;
	key_handler = new CIntroKeyEvent(this); //ngáo :v
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CIntroPlayScene::_ParseSection_SPRITES(string line)
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

void CIntroPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CIntroPlayScene::_ParseSection_ANIMATIONS(string line)
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
void CIntroPlayScene::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_INTRO_LAYER:
		obj = new CIntroLayer(x, y);
		break;

	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int is_UdwTube = atoi(tokens[9].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, is_UdwTube
		);
	}
	break;

	case OBJECT_TYPE_RED_CURTAIN:
		obj = new CRedCurtain(x, y);
		break;
	}
	
	obj->SetPosition(x, y);
	objects.push_back(obj);
	//Đầu tiên là nền đen và Ground Juve: 0
	//Nền lót 0 đc vẽ nên 0 care: 1
	//Rèm đỏ "che" nền đen: 2
	//2 Ae "che" nền đen: 4
	//Rèm SMB3 cùng 1 đống quái: 1 (Luigi.cpp line 255)
	//=>Chèn thằng số 3 racoon vào vị trí 2 =>(begin + 3)

}

void CIntroPlayScene::LoadAssets(LPCWSTR assetFile)
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

void CIntroPlayScene::Load()
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
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CIntroPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	if (!hasUpdateArrowPos)
	{
		CRedArrow::GetInstance()->Update();
		hasUpdateArrowPos = 1;
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	//if (player == NULL) return;

	PurgeDeletedObjects();
}

void CIntroPlayScene::Render()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	//Chỉ vẽ mũi tên sau khi hết màn giới thiệu
	if (allowUseArrow)
		CRedArrow::GetInstance()->Render();
}

void CIntroPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	//Vì là biến tĩnh nên cần reset cái đống này khi Unload Scene
	allowGoombaToMove = 0;
	allowLuigiToThrowKoopa = 0;
	allowReleaseMarioSit = 0;
	allowMarioRun = 0;
	allowLuigiToRunRight = 0;
	LuigiHasRunRight = 0;
	hasTurnSmall = 0;
	allowUseArrow = 0;
	hasUpdateArrowPos = 0;
	LuigiHasThrowKoopa = 0;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void CIntroPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

bool CIntroPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CIntroPlayScene::PurgeDeletedObjects()
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
		std::remove_if(objects.begin(), objects.end(), CIntroPlayScene::IsGameObjectDeleted),
		objects.end());
}