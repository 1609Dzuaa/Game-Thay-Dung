#pragma once
#include "GameObject.h"
#include "Grass.h"

#define SPEED_MARIO 0.1f
#define MARIO_WORLD_MAP_BBOX_WIDTH 8
#define MARIO_WORLD_MAP_BBOX_HEIGHT 8

#define ID_ANI_MARIO_SMALL_AT_WORLD 1795
#define ID_ANI_MARIO_BIG_AT_WORLD	1070
#define ID_ANI_MARIO_RACOON_AT_WORLD 2970

#define MARIO_STATE_NOT_MOVE 100
#define MARIO_STATE_GO_LEFT 200
#define MARIO_STATE_GO_TOP 300
#define MARIO_STATE_GO_RIGHT 400
#define MARIO_STATE_GO_UNDER 500

#define MARIO_STATE_GO_WORLD_1 600

#define TIME_DISPLAY_HUD_SUB 3000

class CWorldMapPlayer : public CGameObject
{
public:
	CWorldMapPlayer(float x, float y) : CGameObject(x, y) 
	{

	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithEntrance(LPCOLLISIONEVENT e);

	void SetState(int state);
};