#include "RedCurtain.h"

void CRedCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedCurtain::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CRedCurtain::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_RED_CURTAIN)->Render(x, y, false);
}

void CRedCurtain::SetState(int state)
{

}