#pragma once
#include "GameObject.h"

//Quite similiar to Mushroom
//Chui từ vật thể ra

#define ATTACK_RANGE 50.0f
#define SHOOTING_FLOWER_RISE_SPEED 0.02f

#define FLOWER_WIDTH 16
#define FLOWER_HEIGHT 32

#define SHOOTING_FLOWER_STATE_IN_TUBE -1
#define SHOOTING_FLOWER_STATE_DIVE 0
#define SHOOTING_FLOWER_STATE_OUT_OF_TUBE 1
#define SHOOTING_FLOWER_STATE_REST 2

#define ID_ANI_FLOWER_RISE_UP_LEFT 81500
#define ID_ANI_FLOWER_RISE_UP_RIGHT 81600
#define ID_ANI_FLOWER_IDLE_LEFT 81550
#define ID_ANI_FLOWER_IDLE_RIGHT 81650
#define ID_ANI_FLOWER_DIVE_LEFT	81700
#define ID_ANI_FLOWER_DIVE_RIGHT 81750

class CShootingFlower :public CGameObject 
{
	float minY, ay;
public:
	CShootingFlower(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int para);
	void Shoot();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(DWORD dt);

	void SetFlowerMinY(float min_Y) { this->minY = min_Y; }
};