#include "GateKeeper.h"

void CGateKeeper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (vx > 0 && x > initX + 10.0f)
		vx = -vx;
	else if(vx < 0 && x < initX - 10.0f)
		vx = -vx;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGateKeeper::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CGateKeeper::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (vx > 0)
		animations->Get(ID_ANI_GATE_KEEPER_MOVE_RIGHT)->Render(x, y, true);
	else
		animations->Get(ID_ANI_GATE_KEEPER_MOVE_LEFT)->Render(x, y, true);
}