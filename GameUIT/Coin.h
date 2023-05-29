#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define COIN_TYPE_IDLE 0
#define COIN_TYPE_FLY 1

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16
#define COIN_FLY_SPEED 0.274f 
//If you found this line, 
//just a small fact that:
//27.4/30 is my University entrance exam score ;)
//the number 0.274 has its reason

class CCoin : public CGameObject 
{
	int type;
	float currentY, minY; //dùng cho loại bay từ QuestionBrick
    //minY: khoảng cách búng đồng tiền lên trên tính từ currentY của QuestionBrick
public:

	CCoin(float x, float y) : CGameObject(x, y) 
	{
		type = COIN_TYPE_IDLE;
		currentY = -1;
		minY = -1;
	}

	CCoin(float x, float y, float vY) : CGameObject(x, y)
	{
		type = COIN_TYPE_FLY;
		this->currentY = y;
		this->minY = currentY - 100.0f;
	}

	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsBlocking() { return 0; } //coin không có thuộc tính block

	void OnNoCollision(DWORD dt);

	void HandleFlyingCoin();
};