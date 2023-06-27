#pragma once
#include "GhostObject.h"
#include "debug.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

#define TAIL_SPEED_X 0.075f 

class CTail : public CGhostObject 
{
	//Tạm để đây làm ôm trc
public:
	CTail(float Mario_posX, float Mario_posY, int MARIO_BBOX_WIDTH, int nx) :CGhostObject(x, y) 
	{
		if (nx > 0)
		{
			this->x = Mario_posX - MARIO_BBOX_WIDTH / 2 - 8.0f;
			this->vx = TAIL_SPEED_X;
		}
		else
		{
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2 + 8.0f;
			this->vx = -TAIL_SPEED_X;
		}
		this->y = Mario_posY + 5.0f;
		
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }

	//All collision_Func
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithFlower(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
};