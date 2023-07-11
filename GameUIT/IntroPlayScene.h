#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include <string>
#include "GameMap.h"
#include "MarioWorld.h"

class CIntroPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CIntroPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void AddObjectToScene(LPGAMEOBJECT game_object) {};

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CIntroPlayScene* LPINTROSCENE;

/*#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include <string>
#include "GameMap.h"
#include "MarioWorld.h"

class CWorldPlayScene : public CScene
{
protected:
	//Coi nó là PlayScene thứ hai
	//Has 2 MAIN Scene To Play:
	//1. World Map
	//2. Entrance of World Map
	LPGAMEOBJECT player;
	CMap* world_map = nullptr;
	BOOLEAN initPos; //Đánh dấu khởi tạo vị trí ở WORLD

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_MAP_MATRIX(string line, CMap*& map_para); //Tham chiếu tới con trỏ

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

	void InitializePositionAtWorld(CMarioWorld* mario); //Hàm khởi tạo vị trí tại World

public:
	static int initStartHud;
	CWorldPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CWorldPlayScene* LPWORLDPLAYSCENE;*/