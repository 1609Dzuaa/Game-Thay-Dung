#pragma once
#include "GhostObject.h"
#include "debug.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

#define TAIL_STATE_IDLE 0
#define TAIL_STATE_ATTACK 1

class CTail : public CGhostObject 
{
	//Tạo đc đuôi là 1 chuyện, phải luôn cập nhật vị trí của nó với Mario
public:
	CTail(float Mario_posX, float Mario_posY, int MARIO_BBOX_WIDTH, int nx) :CGhostObject(x, y) 
	{
		if (nx > 0)
			this->x = Mario_posX - MARIO_BBOX_WIDTH / 2;
		else
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2;
		this->y = Mario_posY;
		this->state = TAIL_STATE_IDLE;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);

	//All collision_Func
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void SetState(int state); //cái đuôi cũng có trạng thái
};