﻿#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include <string>
#include "GameMap.h"

class CWorldPlayScene : public CScene
{
protected:
	//Coi nó là PlayScene thứ hai
	//Has 2 MAIN Scene To Play:
	//1. World Map
	//2. Entrance of World Map
	LPGAMEOBJECT player;
	CMap* world_map = nullptr;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_MAP_MATRIX(string line, CMap*& map_para); //Tham chiếu tới con trỏ

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
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

typedef CWorldPlayScene* LPWORLDPLAYSCENE;