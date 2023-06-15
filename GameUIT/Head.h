#pragma once
#include "GhostObject.h"
#include "ColorPlatform.h"

#define HEAD_BBOX_WIDTH	5
#define HEAD_BBOX_HEIGHT	5

class CHead :public CGhostObject
{
	float ay;
	bool isFallOff;
public:
	CHead(float x, float y, float koopa_vx, float koopa_grav) :CGhostObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->ay = koopa_grav;
		this->vx = koopa_vx;
		isFallOff = 0;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);
	int GetIsFallOff() { return isFallOff; }
	void SetIsFallOff(bool para) { this->isFallOff = para; }
};