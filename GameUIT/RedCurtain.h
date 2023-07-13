#pragma once
#include "GameObject.h"

//#define RED_CURTAIN_SPEED_UP	0.075f
#define RED_CURTAIN_SPEED_UP	0.1f

#define ID_ANI_RED_CURTAIN	500001

class CRedCurtain : public CGameObject
{
	BOOLEAN HasSpawned;
public:
	CRedCurtain(float x, float y) :CGameObject(x, y) 
	{ 
		vy = -RED_CURTAIN_SPEED_UP;
		HasSpawned = false;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};

	void SpawnMarioAndLuigi();
};