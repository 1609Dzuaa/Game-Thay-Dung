#pragma once
#include "GameObject.h"

#define TUBE_BBOX_WIDTH		31
#define TUBE_BBOX_HEIGHT	31

#define TUBE_TYPE_SHOOTING_FLOWER 1


#define ATTACK_ZONE 250.0f

#define ID_ANI_TUBE 5500

class CTube :public CGameObject 
{
	int Type;
public:
	CTube(float x, float y, int type) :CGameObject(x, y) { this->Type = type; };

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects); //gọi hoa trong hàm update
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	void SpawnShootingFlower();
};