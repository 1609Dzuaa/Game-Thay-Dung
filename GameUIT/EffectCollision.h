#pragma once
#include "Effect.h"

#define EFFECT_COLLISION_TYPE_NORMAL 0
#define EFFECT_COLLISION_TYPE_SMOKE 1

#define ANI_ID_EFFECT_COLLISION 15
#define ANI_ID_EFFECT_COLLISION_SMOKE 20

#define EFFECT_COLLISION_TIME_OUT 180

//Vụt cây sinh ra cả hiệu ứng vụt lẫn hiệu ứng khói

class CEffectCollision : public CEffect 
{
	ULONGLONG effect_start;
public:
	CEffectCollision(float x, float y, ULONGLONG eff_col_start) :CEffect(x, y) 
	{ 
		this->effect_start = eff_col_start; 
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};