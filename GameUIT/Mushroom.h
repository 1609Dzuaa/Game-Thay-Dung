#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_X 0.075f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_RISE_UP_SPEED 0.016f
#define MUSHROOM_STATE_IN_THE_BRICK 0
#define MUSHROOM_STATE_OUT_OF_BRICK 10

#define MUSHROOM_RISEN_UP_TIME 300

#define ID_ANI_MUSHROOM_IN_BRICK 9000
#define ID_ANI_MUSHROOM_OUT_BRICK 9001

class CMushroom : public CGameObject 
{
	//2 loại Nấm: Đỏ và Xanh
	float ax, ay, brickminY;
public:
	CMushroom(float x, float y, float brickY, ULONGLONG Hitted_time) : CGameObject(x, y)
	{
		this->vx = 0;
		this->ax = 0;
		this->ay = MUSHROOM_GRAVITY;
		this->vy = MUSHROOM_RISE_UP_SPEED;
		brickminY = brickY;
		state = MUSHROOM_STATE_IN_THE_BRICK; //Ban đầu sẽ ở trong cục gạch
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();

	int IsBlocking() { return 0; }

	int IsCollidable() { return 1; };

	void OnNoCollision(DWORD dt);

	void OnCollisionWith(LPCOLLISIONEVENT e);

	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);

	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetBrickMinY(float para) { this->brickminY = para; }

	void SetMushroomX(float x) { this->x = x; }

	void SetState(int state);
};