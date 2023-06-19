#include "EffectCollision.h"

void CEffectCollision::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->type == EFF_COL_TYPE_NORMAL)
	{
		if (GetTickCount64() - effect_start >= EFFECT_COLLISION_NORMAL_TIME_OUT)
			this->Delete();
	}
	else 
	{
		if (GetTickCount64() - effect_start >= EFFECT_COLLISION_SMOKE_TIME_OUT)
			this->Delete();
	}
}

void CEffectCollision::Render()
{
	int aniID = GetAniEffect();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y, false);
}

int CEffectCollision::GetAniEffect()
{
	int ani = -1;
	if (this->type == EFF_COL_TYPE_NORMAL)
		ani = ANI_ID_EFFECT_COLLISION_NORMAL;
	else if (this->type == EFF_COL_TYPE_SMOKE_EVOLVE)
		ani = ANI_ID_EFFECT_COLLISION_SMOKE;

	return ani;
}