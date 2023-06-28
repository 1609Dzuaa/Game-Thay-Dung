#pragma once
#include "Effect.h"

#define EFFECT_SCORE_VELO_Y 0.03f

#define NORMAL_SCORE 10
#define DOUBLE_SCORE 20
#define QUADRA_SCORE 40
#define DOUBLE_QUADRA_SCORE 80
#define ITEM_SCORE 100 //Mushroom, Leaf
#define ITEM_SCORE_LV_UP	110

class CEffectScore :public CEffect 
{
	float minY;
	int aniId;
public:
	CEffectScore(float x, float y, float min_Y, int ani_ID) :CEffect(x, y) 
	{
		this->minY = min_Y;
		this->vy = -EFFECT_SCORE_VELO_Y;
		this->aniId = ani_ID;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};