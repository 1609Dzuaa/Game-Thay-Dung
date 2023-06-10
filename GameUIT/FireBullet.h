#pragma once
#include "GameObject.h"

#define FIRE_BULLET_GRAVITY 0.00009f
#define FIRE_BULLET_VX 0.06f

#define FIRE_BULLET_BBOX_HEIGHT 9
#define FIRE_BULLET_BBOX_WIDTH 8

#define ID_ANI_FIRE_BULLET 5900

class CFireBullet :public CGameObject 
{
	float ax, ay;
public:
	CFireBullet(float x, float y)
	{
		ax = 0;
		ay = FIRE_BULLET_GRAVITY;
		vx = FIRE_BULLET_VX;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	int IsBlocking() { return 0; }
	void SetVX(float v_x) { this->vx = v_x; }
	//int IsCollidable() { return 1; }
	//Đạn thì 0 có thuộc tính Block nhưng có khả năng va chạm
};