#pragma once
#include "Effect.h"

#define EFFECT_SCORE_VELO_Y 0.03f

#define ID_ANI_SCORE_EFFECT_100 100

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