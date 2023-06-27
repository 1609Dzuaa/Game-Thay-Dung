#pragma once
#include "GhostObject.h"
#include "debug.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

#define TAIL_SPEED_X_MARIO_IDLE 0.076f 
#define TAIL_SPEED_X_MARIO_MOVE	0.09f
//đang move thì cho vận tốc khác để bắt kịp vị trí ?
//currently use this way

class CTail : public CGhostObject 
{
public:
	CTail(float Mario_posX, float Mario_posY, int MARIO_BBOX_WIDTH, int nx, float mario_vx);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//void UpdateTailPos();
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