#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "EffectCollision.h"
#include "Switch.h"
#include "Mushroom.h"

#define ID_ANI_STRIPE_BRICK 10000
#define ID_ANI_GOLD_BRICK 20200
#define	ID_ANI_GOLD_COIN	20250 

#define GOLD_COIN_TIME_OUT 5500

#define BRICK_WIDTH 17
#define BRICK_BBOX_WIDTH 17
#define BRICK_BBOX_HEIGHT 17

#define GOLD_BRICK_BOUNCING_SPEED 0.05f
#define GOLD_BRICK_GRAVITY	0.05f

#define GBRICK_HAS_ITEM_STATE_IS_HITTED	1	//Only use for Gold Brick that has Switch: bị hit thì spawn khói
#define GBRICK_STATE_NORMAL	2
#define GBRICK_STATE_TURN_TO_COIN	3
#define GBRICK_STATE_IS_DESTROYED	4

//chỉ level > SMALL thì mới phá huỷ đc GOLD brick
#define GOLD_BRICK 1	//Kh thực sự xoá đi gold brick mà biến nó thành vàng
#define STRIPE_BRICK 2

#define NO_ITEM	0
#define SWITCH 5
#define MUSHROOM	10

class CBrick : public CGameObject 
{
	int type, item_type; //re-check again ?!
	float ay, old_pos, min_pos;
	bool isBeingHitted, isTurnToCoin;
	CEffectCollision* eff;
	CSwitch* _switch;
	CMushroom* _mushroom;
	ULONGLONG hit_start, gold_coin_start;
public:
	CBrick(float x, float y, int type, int item_type);
	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdatePosition(DWORD dt);
	void UpdateGoldCoin();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetType() { return this->type; }
	int GetItemType() { return this->item_type; }

	void OnNoCollision(DWORD dt);
	int IsBlocking() { return state != GBRICK_STATE_TURN_TO_COIN && state != GBRICK_STATE_IS_DESTROYED; } //bị biến thành gold coin thì 0 block
	void SetHitted(bool para) { this->isBeingHitted = para; }
	void SetState(int state);
	void SpawnSwitch();
	void SpawnMushroom();
	void SpawnBrickPiece();//finish this
};