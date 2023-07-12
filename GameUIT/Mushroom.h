#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_X 0.04f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_RISE_UP_SPEED 0.016f
#define MUSHROOM_STATE_IN_THE_BRICK 0
#define MUSHROOM_STATE_RISE_UP 5
#define MUSHROOM_STATE_OUT_OF_BRICK 10

#define MUSHROOM_RISEN_UP_TIME 300

#define ID_ANI_GREEN_MUSHROOM 9000
#define ID_ANI_RED_MUSHROOM 9001

#define RED_MUSHROOM	1
#define GREEN_MUSHROOM	2

class CMushroom : public CGameObject 
{
	//2 loại Nấm: Đỏ và Xanh
	float ax, ay, brickminY;
	int type;
public:
	CMushroom(float x, float y, float brickmin_Y, int type) : CGameObject(x, y)
	{
		this->vx = 0;
		this->ax = 0;
		this->ay = MUSHROOM_GRAVITY;
		this->type = type;
		this->vy = 0;
		brickminY = brickmin_Y;
		state = MUSHROOM_STATE_IN_THE_BRICK; //Ban đầu sẽ ở trong cục gạch -> không vẽ
		IsWaitable = true;
		IsWaiting = false;	//Chú ý thằng này
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();

	int IsBlocking() { return 0; }

	int IsCollidable() { return state != MUSHROOM_STATE_IN_THE_BRICK; };

	void OnNoCollision(DWORD dt);

	void OnCollisionWith(LPCOLLISIONEVENT e);

	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);

	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int GetType() { return this->type; }

	void SetBrickMinY(float para) { this->brickminY = para; }

	void SetMushroomX(float x) { this->x = x; }

	void SetState(int state);

	int IsSpecialItem() { return 1; }
};