#pragma once
#include"GameObject.h"


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_X 0.1f
#define MUSHROOM_ACCELERATION_X 0.0005f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_RISE_UP_SPEED 0.009f
#define IN_THE_BRICK 0
#define OUT_OF_BRICK 10

#define MUSHROOM_RISEN_UP_TIME 150

#define ID_ANI_MUSHROOM_IN_BRICK 9000
#define ID_ANI_MUSHROOM_OUT_BRICK 9001

class CMushroom : public CGameObject 
{
	//2 loại Nấm
	float ax, ay, brickminY;
	ULONGLONG hitted_time;
public:
	CMushroom(float x, float y, float brickY, ULONGLONG Hitted_time) : CGameObject(x, y)
	{
		this->vx = 0;
		this->ax = 0;
		this->ay = MUSHROOM_GRAVITY;
		this->vy = MUSHROOM_RISE_UP_SPEED;
		brickminY = brickY;
		state = IN_THE_BRICK;
		hitted_time = Hitted_time;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

	virtual int IsBlocking() { return 0; }

	virtual int IsCollidable() { return 1; };

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetBrickMinY(float para) { this->brickminY = para; }

	void SetMushroomX(float x) { this->x = x; }

	void SetState(int state);
};