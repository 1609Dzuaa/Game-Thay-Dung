#pragma once
#include "GameObject.h"

//Quite similiar to Mushroom
//Chui từ vật thể ra

#define ATTACK_RANGE 150.0f

#define SHOOTING_FLOWER_STATE_IN_TUBE -1
#define SHOOTING_FLOWER_STATE_RISE_UP 0
#define SHOOTING_FLOWER_STATE_IDLE 1

#define ID_ANI_FLOWER_RISE_UP_LEFT 81500
#define ID_ANI_FLOWER_RISE_UP_RIGHT 81600
#define ID_ANI_FLOWER_IDLE_LEFT 81550
#define ID_ANI_FLOWER_IDLE_RIGHT 81650

class CShootingFlower :public CGameObject 
{
public:
	CShootingFlower();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int para);
	void Shoot();

	//virtual void OnNoCollision(DWORD dt);

	//virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};