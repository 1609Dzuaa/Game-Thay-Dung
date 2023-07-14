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
	//LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;

	int Priority;//Mức ưu tiên => dùng để xác định vật nào đc vẽ trc, tránh "đè" vật khác

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

	void AddObjectToScene(LPGAMEOBJECT game_object, int priority) 
	{
		this->Priority = priority;
		if (Priority == 0)
			objects.push_back(game_object);
		else if (Priority == 1) //Why 1 ? => Dùng cho rèm SMB3 with Color để nó 0 vẽ "đè" Mario
			objects.insert(objects.begin() + 1, game_object);
		else if(Priority == 2) //Why 2 ? => Dùng cho số 3 Racoon để nó vẽ "đè" số 3 trong rèm SMB3
			objects.insert(objects.begin() + 3, game_object);
	};

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	static bool allowGoombaToMove; //Cờ để Goomba nhận biết và di chuyển
	static bool allowReleaseMarioSit; //Lúc Luigi đã kéo màn VÀ đi quá màn hình thì cho Mario hết ngồi
	static bool allowMarioRun;	//Lúc Luigi đi qua vạch thì cho mario chạy
	static bool allowLuigiToThrowKoopa; //Cho phép Luigi tạo Koopa, ôm và đạp nó
	static bool allowLuigiToRunRight; //Cho phép Luigi chạy đi mất
	static bool LuigiHasRunRight; //dùng để spawn cái mai rùa từ hướng trái
	static bool hasTurnSmall; //dùng để biết Mario đã bị hoá nhỏ và cho nó di chuyển
	static bool allowUseArrow;
	static bool hasUpdateArrowPos;
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