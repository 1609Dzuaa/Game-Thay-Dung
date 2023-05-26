#pragma once
#include"GameObject.h"


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_X 0.1f
#define MUSHROOM_ACCELERATION_X 0.0005f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_RISE_UP_SPEED 0.015f
#define OUT_OF_BRICK 10


#define ID_ANI_MUSHROOM 9000

class CMushroom : public CGameObject 
{
	//2 loại Nấm
	float ax, ay, brickminY;
	int untouchable;
	ULONGLONG untouchable_start;
	bool isOutOfBrick;
public:
	CMushroom(float x, float y, float brickY) : CGameObject(x, y)
	{
		this->vx = MUSHROOM_SPEED_X;
		this->ax = MUSHROOM_ACCELERATION_X;
		this->ay = MUSHROOM_GRAVITY;
		untouchable = 1;
		untouchable_start = 0;
		brickminY = brickY;
		isOutOfBrick = false;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

	virtual int IsBlocking() { return 0; }

	virtual int IsCollidable() { return 1; };

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetBrickMinY(float para) { this->brickminY = para; }

	void SetState(int state);
};