#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define LEAF_FLY_SPEED 0.2f
#define LEAF_FALL_SPEED_Y 0.01f
#define LEAF_FALL_SPEED_X 0.02f
#define LEAF_GRAVITY 0.00001f

#define ID_ANI_LEAF_RIGHT 80000
#define ID_ANI_LEAF_LEFT 81000
#define LEAF_WIDTH 16
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16


class CLeaf : public CGameObject 
{
	float ax, ay, minY, minX, maxX;
public:
	CLeaf(float x, float y) : CGameObject(x, y) 
	{
		this->ax = 0;
		this->ay = 0;
		vy = -LEAF_FLY_SPEED;
		minY = -1;
		minX = x;
		maxX = x + 31.0f;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetMinY(float para) { this->minY = para; }
	void setMinX(float para) { this->minX = para; }
};