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

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;
	CMap* map = nullptr;
	CMap* underworld_map = nullptr; //được giấu ở dưới theo trục y
	static int timer;
	ULONGLONG timer_start;
	BOOLEAN IsWait;
	BOOLEAN init;
	int prevHP;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_MAP_MATRIX(string line, CMap* &map_para); //Tham chiếu tới con trỏ

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Reload(CMario* mario, LPGAMEOBJECT obj);
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void AddObjectToScene(LPGAMEOBJECT game_object);
	int GetTimer() { return timer; }
	BOOLEAN GetWait() { return IsWait; }
	void SetWait(BOOLEAN para) { this->IsWait = para; }
	void SetInit(BOOLEAN para) { this->init = para; }
	void HandleTimerAndWait();
};

typedef CPlayScene* LPPLAYSCENE;