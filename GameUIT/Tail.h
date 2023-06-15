#pragma once
#include "GhostObject.h"
#include "debug.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

class CTail : public CGhostObject 
{
	//Tạo đc đuôi là 1 chuyện, phải luôn cập nhật vị trí của nó với Mario
public:
	CTail(float Mario_posX, float Mario_posY) :CGhostObject(x, y) 
	{
		this->x = Mario_posX + 14 / 2;
		this->y = Mario_posY + 24 / 4;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);

	//All collision_Func
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
};