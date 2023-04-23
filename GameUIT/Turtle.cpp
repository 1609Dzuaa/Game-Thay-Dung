#include "Game.h"
#include "Turtle.h"

CTurtle::CTurtle(float x, float y, float vx) :CGameObject(x, y)
{
	this->vx = vx;
}

void CTurtle::Update(DWORD dt)
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

void CTurtle::Render()
{
	LPANIMATION ani;

	ani = CAnimations::GetInstance()->Get(503);
	ani->Render(x, y);
}