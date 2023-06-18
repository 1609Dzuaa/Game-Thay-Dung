#pragma once
#include "Effect.h"

#define EFF_COL_TYPE_NORMAL 0 //hiệu ứng vụt
#define EFF_COL_TYPE_SMOKE_EVOLVE 1 //hiệu ứng khói

#define ANI_ID_EFFECT_COLLISION_NORMAL 15
#define ANI_ID_EFFECT_COLLISION_SMOKE 20

#define EFFECT_COLLISION_NORMAL_TIME_OUT 180
#define EFFECT_COLLISION_SMOKE_TIME_OUT 750 //mò cả sáng hoá ra vấn đề ở đây :)

//Vụt cây sinh ra cả hiệu ứng vụt lẫn hiệu ứng khói:
//Muốn kết hợp nó thì spawn khói khi cây CHẾT và spawn vụt khi gấu mèo quẫy đuôi

class CEffectCollision : public CEffect 
{
	ULONGLONG effect_start;
	int type;
public:
	CEffectCollision(float x, float y, ULONGLONG eff_col_start, int type) :CEffect(x, y) 
	{ 
		this->effect_start = eff_col_start; 
		this->type = type;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	int GetAniEffect();
};