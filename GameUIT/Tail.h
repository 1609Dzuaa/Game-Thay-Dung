#pragma once
#include "GhostObject.h"
#include "debug.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

#define TAIL_STATE_IDLE 0
#define TAIL_STATE_ATTACK 1

class CTail : public CGhostObject 
{
	//Tạm để đây làm ôm trc
public:
	CTail(float Mario_posX, float Mario_posY, int MARIO_BBOX_WIDTH, int nx) :CGhostObject(x, y) 
	{
		if (nx < 0)
			this->x = Mario_posX - MARIO_BBOX_WIDTH / 2 - 6.0f;
		else
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2 + 6.0f;
		this->y = Mario_posY + 5.0f;
		this->state = TAIL_STATE_ATTACK;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);

	//All collision_Func
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithShootingFlower(LPCOLLISIONEVENT e);
	void SetState(int state); //cái đuôi cũng có trạng thái
};