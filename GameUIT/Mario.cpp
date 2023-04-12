#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float vx) :CGameObject(x, y)
{
	this->vx = vx;
	ID = 500;
};

void CMario::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH)
	{

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(500); //lúc đi
	else ani = CAnimations::GetInstance()->Get(501); //lúc về

	ani->Render(x, y);
}