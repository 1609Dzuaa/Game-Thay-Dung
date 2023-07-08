#pragma once
#include "GameObject.h"
#include "Grass.h"

#define SPEED_MARIO 0.1f
#define MARIO_WORLD_MAP_BBOX_WIDTH 8
#define MARIO_WORLD_MAP_BBOX_HEIGHT 8

#define ID_ANI_MARIO_SMALL_AT_WORLD 1795
#define ID_ANI_MARIO_BIG_AT_WORLD	1070
#define ID_ANI_MARIO_RACOON_AT_WORLD 2970

#define MARIO_WORLD_STATE_MOVE_RIGHT	100
#define MARIO_WORLD_STATE_MOVE_LEFT	200
#define MARIO_WORLD_STATE_MOVE_UP	300
#define MARIO_WORLD_STATE_MOVE_DOWN	400
#define MARIO_WORLD_STATE_ENTER_ENTRANCE	500

class CMarioWorld : public CGameObject
{
	BOOLEAN isAllowToPlayThatEntrance;
	BOOLEAN HasCollidedWithEntrance;
	BOOLEAN isMoving;
	vector<int> EntranceHasPassed;
	int Entrance_Type;
	D3DXVECTOR2 Entrance_Position;
public:
	CMarioWorld(float x, float y) : CGameObject(x, y)
	{
		isAllowToPlayThatEntrance = false;
		HasCollidedWithEntrance = false;
		isMoving = false;
		Entrance_Position.x = 0;
		Entrance_Position.y = 0;
		Entrance_Type = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithEntrance(LPCOLLISIONEVENT e);
	void HandlePositionWithEntrance(); //Xử lý vị trí của Mario với Entrance
	bool IsPassedThisEntrance(int entr_type);
	void SetState(int state);
	bool GetIsMoving() { return this->isMoving; }
};