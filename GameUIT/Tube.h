#pragma once
#include "GameObject.h"

#define TUBE_BBOX_WIDTH		31
#define TUBE_BBOX_HEIGHT	47

#define TUBE_TYPE_SHOOTING_FLOWER 1


#define TUBE_STATE_SPAWN_FLOWER 1
#define TUBE_STATE_STOP_SPAWN 2

#define ATTACK_ZONE 70.0f

#define ID_ANI_TUBE 5500

class CTube :public CGameObject 
{
	int Type;
	BOOLEAN Flower_Arise;
public:
	CTube(float x, float y, int type) :CGameObject(x, y) 
	{ 
		this->Type = type; 
		this->Flower_Arise = false;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects); //gọi hoa trong hàm update
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	void SpawnShootingFlower();
	void SetState(int state);
};