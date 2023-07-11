#include "LuigiNPC.h"
#include "ColorPlatform.h"

void CLuigiNPC::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLuigiNPC::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
}

void CLuigiNPC::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLuigiNPC::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	//animations->Get()->Render(x, y, false);
}

void CLuigiNPC::SetState(int state)
{

}