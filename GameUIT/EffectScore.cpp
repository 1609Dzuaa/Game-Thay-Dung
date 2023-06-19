#include "EffectScore.h"

void CEffectScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->y <= minY)
		this->Delete();
	else
		y += vy * dt;
}

void CEffectScore::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(this->aniId)->Render(x, y, false);
}