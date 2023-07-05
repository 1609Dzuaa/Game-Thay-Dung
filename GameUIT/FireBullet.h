#pragma once
#include "GameObject.h"

#define FIRE_BULLET_VX 0.06f //nên dựa vào vị trí Mario chứ không nên dựa cứng vào hằng số như này
#define FIRE_BULLET_VY 0.03f

#define FIRE_BULLET_BBOX_HEIGHT 9
#define FIRE_BULLET_BBOX_WIDTH 8

#define ID_ANI_FIRE_BULLET 5900

class CFireBullet :public CGameObject 
{
public:
	CFireBullet(float x, float y)
	{
		vx = FIRE_BULLET_VX;
		vy = FIRE_BULLET_VY;
		IsWaitable = true;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	int IsBlocking() { return 0; }
	//int IsCollidable() { return 1; }
	//Đạn thì 0 có thuộc tính Block nhưng có khả năng va chạm
};