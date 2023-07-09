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
	BOOLEAN HasCollidedWithEntrance, HasCollidedWithBlock;
	BOOLEAN isMoving;
	vector<int> EntranceHasPassed;
	int Entrance_ID;
	D3DXVECTOR2 Entrance_Position;
	D3DXVECTOR4 Direct_Been_Blocked; //Nếu chưa qua entrance thì entrance đó block hết các hướng còn lại
	BOOLEAN isTravelling;
	BOOLEAN init;
	//Thu hẹp ViewPort ?
public:
	CMarioWorld(float x, float y) : CGameObject(x, y)
	{
		isAllowToPlayThatEntrance = false;
		HasCollidedWithEntrance = false; //Để nhận biết có thể chuyển Scene
		HasCollidedWithBlock = false;
		isMoving = false;
		Entrance_Position.x = 0;
		Entrance_Position.y = 0;
		Entrance_ID = 0;
		//Ban đầu vị trí ở Start(Block L, T, B) -> obviously
		Direct_Been_Blocked.x = 1;
		Direct_Been_Blocked.y = 1;
		Direct_Been_Blocked.w = 1;
		isTravelling = 0;
		init = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithEntrance(LPCOLLISIONEVENT e);
	void OnCollisionWithBlockPoint(LPCOLLISIONEVENT e);
	void HandlePositionWithEntranceAndBlock(); //Xử lý vị trí của Mario với Entrance và Block
	bool IsPassedThisEntrance(int entr_type);
	void SetState(int state);
	bool GetIsMoving() { return this->isMoving; }
	bool GetIsAllowEnter() { return this->isAllowToPlayThatEntrance; }
	D3DXVECTOR4 GetBlockDirect() { return this->Direct_Been_Blocked; }
};