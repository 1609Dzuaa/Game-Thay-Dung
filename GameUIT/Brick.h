#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "EffectCollision.h"
#include "Switch.h"

#define ID_ANI_STRIPE_BRICK 10000
#define ID_ANI_GOLD_BRICK 20200
#define	ID_ANI_GOLD_COIN	20250 

#define GOLD_COIN_TIME_OUT 5500

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define GOLD_BRICK_BOUNCING_SPEED 0.05f
#define GOLD_BRICK_GRAVITY	0.05f

#define GOLD_BRICK_HAS_SW_STATE_IS_HITTED	1	//Only use for Gold Brick that has Switch: bị hit thì spawn khói
#define GOLD_BRICK_STATE_NORMAL	2
#define GOLD_BRICK_STATE_TURN_TO_COIN	3

//chỉ level > SMALL thì mới phá huỷ đc GOLD brick
#define GOLD_BRICK 1	//Kh thực sự xoá đi gold brick mà biến nó thành vàng
#define STRIPE_BRICK 2

class CBrick : public CGameObject 
{
	int type, hasSwitch; //re-check again ?!
	float ay, old_pos, min_pos;
	bool isBeingHitted, isTurnToCoin;
	CEffectCollision* eff;
	CSwitch* _switch;
	ULONGLONG hit_start, gold_coin_start;
public:
	CBrick(float x, float y, int type, int hasSwitch) : CGameObject(x, y)
	{
		this->type = type;
		this->hasSwitch = hasSwitch;
		this->_switch = NULL;
		ay = GOLD_BRICK_GRAVITY;
		old_pos = this->y;
		min_pos = this->y - 10.0f;
		isBeingHitted = false;
		isTurnToCoin = false;
		hit_start = -1;
		gold_coin_start = -1;
		eff = NULL;
	}
	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdatePosition(DWORD dt);
	void UpdateGoldCoin();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetType() { return this->type; }
	int GetHasSwitch() { return this->hasSwitch; }

	void OnNoCollision(DWORD dt);
	int IsBlocking() { return state != GOLD_BRICK_STATE_TURN_TO_COIN; } //bị biến thành gold coin thì 0 block
	void SetHitted(bool para) { this->isBeingHitted = para; }
	void SetState(int state);
	void SpawnSwitch();
};