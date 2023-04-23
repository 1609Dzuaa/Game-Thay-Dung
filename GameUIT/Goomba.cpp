#include "Game.h"
#include "Goomba.h"

CGoomba::CGoomba(float x, float y, float vx) :CGameObject(x, y)
{
	this->vx = vx;
}

void CGoomba::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - 14.0f)
	{

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - 14.0f)
		{
			x = (float)(BackBufferWidth - 14.0f);
		}
	}
}

void CGoomba::Render()
{
	LPANIMATION ani;

	//if(die) get(Goomba_Die) ani->Render(x,y)
	ani = CAnimations::GetInstance()->Get(502);
	ani->Render(x,y);
}