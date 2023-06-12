#pragma once
#include "Effect.h"

#define ANI_ID_EFFECT_COLLISION 15

#define EFFECT_COLLISION_TIME_OUT 180

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