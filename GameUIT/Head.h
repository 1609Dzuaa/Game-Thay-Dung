#pragma once
#include "GhostObject.h"
#include "ColorPlatform.h"

#define HEAD_BBOX_WIDTH	14
#define HEAD_BBOX_HEIGHT	14

#define HEAD_STATE_MOVE	0
#define HEAD_STATE_IDLE	1	

#define FALL_ZONE 7.0f

//có lẽ nên chuyển sang từ va chạm với blocking objects thành xét y, nếu y của nó thay đổi đột ngột thì quay đầu
//How 'bout comparison Y between ghost_head and koopa 
//Tìm cách mà nó bao quát hết mọi trường hợp

class CHead :public CGhostObject
{
	float ay;
	bool isFallOff;
public:
	CHead(float x, float y, float koopa_vx, float koopa_grav) :CGhostObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->vx = koopa_vx;
		this->ay = koopa_grav;
		isFallOff = false;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);
	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);
	bool GetIsFallOff() { return isFallOff; }
	void SetIsFallOff(bool para) { this->isFallOff = para; }
	void SetState(int state);
	float GetVy() { return this->vy; }
};