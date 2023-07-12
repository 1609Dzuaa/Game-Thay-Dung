#pragma once
#include "GameObject.h"

#define STAR_INTRO_BOUNCING_SPEED	0.35f

#define STAR_INTRO_BBOX_WIDTH	15
#define STAR_INTRO_BBOX_HEIGHT	15

class CStarIntro : public CGameObject
{
	float ay;
public:
	CStarIntro(float x, float y) :CGameObject(x, y)
	{
		ay = 0.0005f;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
