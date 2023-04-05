#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

//File mã nguồn (.cpp) chứa các định nghĩa và mã lệnh

/*
	Initialize game object
*/

CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

CBall::~CBall()
{
	if (this->texture != NULL) delete this->texture;
}

#define MARIO_VX 0.1f
#define MARIO_WIDTH 14

#define BALL_VX 0.1f
#define BALL_WIDTH 4

void CMario::Update(DWORD dt)
{
	x += vx * dt;
	y -= vy * dt;

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
	if (y < 5 || y > 100)
		vy = -vy;
}

void CBall::Update(DWORD dt)
{
	if (this->texture != NULL)
	{
		//x += vx * dt;
		y += vy * dt;
		//int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
		int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
		/*if (x <= 0 || x >= BackBufferWidth)
		{

			vx = -vx;

			if (x <= 0)
			{
				x = 0;
			}
			else if (x >= BackBufferWidth - BALL_WIDTH)
			{
				x = (float)(BackBufferWidth - BALL_WIDTH);
			}
		}*/
		if (y <= 0 || y >= BackBufferHeight)
		{
			vy = -vy;
			if (y <= 0)
			{
				y = 0;
			}
			else if (y > BackBufferHeight)
			{
				y = (float)(BackBufferHeight);
			}
		}
	}
}

void CBrick::Update(DWORD dt)
{
	x += vx * dt;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth)
	{
		vx = -vx;
		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth)
		{
			x = (float)(BackBufferWidth);
		}
	}
}

void CTurtle::Update(DWORD dt)
{
	x += vx * dt;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth)
	{
		vx = -vx;
		if (x < 0)
			x = 0;
		else if (x > BackBufferWidth)
			x = (float)BackBufferWidth;
	}
}