#pragma once
#include "GhostObject.h"
//#include "Mario.h"

#define TAIL_BBOX_WIDTH	10
#define TAIL_BBOX_HEIGHT 6

class CTail : public CGhostObject 
{
public:
	CTail(float Mario_posX, float Mario_posY) :CGhostObject(x, y) 
	{
		this->x = Mario_posX + 14 / 2;
		this->y = Mario_posY + 24 / 4;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render() { RenderBoundingBox(); };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
	//void RenderBoundingBox();

	//All collision_Func
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
};