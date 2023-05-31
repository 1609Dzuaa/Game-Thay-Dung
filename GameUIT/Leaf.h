#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define LEAF_FLY_SPEED 0.2f
#define LEAF_FALL_DOWN_SPEED_Y 0.02f
#define LEAF_FALL_DOWN_SPEED_X 0.025f
#define LEAF_GRAVITY 0.002f
#define LEAF_DRAG 0.2f

#define LEAF_STATE_FALL_DOWN 0 
#define LEAF_STATE_FALL_UP 1
#define LEAF_STATE_FLY_AWAY_FROM_BRICK 2

#define LEAF_FALL_UP_TIME 100

#define ID_ANI_LEAF_RIGHT 80000
#define ID_ANI_LEAF_LEFT 81000
#define LEAF_WIDTH 16
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16


class CLeaf : public CGameObject 
{
	float ax, ay, minY, minX, maxX;
	bool reachMinOrMax_X;
	ULONGLONG Fall_Up_start;
public:
	CLeaf(float x, float y);

	int IsBlocking() { return 0; };

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetMinY(float para) { this->minY = para; }
	void setMinX(float para) { this->minX = para; }
	void SetState(int state);
};