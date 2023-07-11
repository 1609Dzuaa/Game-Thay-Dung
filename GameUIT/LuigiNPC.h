#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"

#define LUIGI_NPC_WALKING_SPEED	0.15f
#define LUIGI_NPC_JUMP_SPEED	0.5f

#define LUIGI_NPC_STATE_IDLE	0
#define LUIGI_NPC_STATE_WALKING	10
#define LUIGI_NPC_STATE_JUMPING	20

#define ID_ANI_BIG_LUIGI_IDLE	

class CLuigiNPC : public CGameObject
{
	float ax, ay;
public:
	CLuigiNPC(float x, float y) :CGameObject(x, y)
	{
		ax = 0.0f;
		ay = 0.01f;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithColorPlatform(LPCOLLISIONEVENT e);
	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);
};
