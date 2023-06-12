#include "EffectCollision.h"

void CEffectCollision::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - effect_start >= EFFECT_COLLISION_TIME_OUT)
		this->Delete();
}

void CEffectCollision::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ANI_ID_EFFECT_COLLISION)->Render(x, y);
}