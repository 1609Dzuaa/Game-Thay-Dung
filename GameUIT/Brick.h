#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_GOLD_BRICK 20200
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define GOLD_BRICK_BOUNCING_SPEED 0.05f
#define GOLD_BRICK_GRAVITY	0.05f

#define GOLD_BRICK 1
#define STRIPE_BRICK 2


class CBrick : public CGameObject 
{
	int type;
	float ay, old_pos, min_pos;
	bool isBeingHitted;
public:
	CBrick(float x, float y, int type) : CGameObject(x, y) 
	{ 
		this->type = type;
		ay = GOLD_BRICK_GRAVITY;
		old_pos = this->y;
		min_pos = this->y - 10.0f;
		isBeingHitted = false;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
	int IsCollidable() { return type != STRIPE_BRICK; }
	int GetType() { return this->type; }
	void SetHitted(bool para) { this->isBeingHitted = para; }
};