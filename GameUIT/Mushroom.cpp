#include "Mushroom.h"
#include "Mario.h"
#include "debug.h"

extern CMario* mario;

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != OUT_OF_BRICK)
	{
		if (y <= brickminY)
		{
			y = brickminY;
			this->SetState(OUT_OF_BRICK);
		}
		else
		{
			y -= MUSHROOM_RISE_UP_SPEED * dt;
		}
	}
	else 
	{
		vy += ay * dt;
		vx += ax * dt;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"State and X and Mario X: %d, %f, %f", state, x, mario->GetMarioPositionX());
}

void CMushroom::OnNoCollision(DWORD dt)
{
	if (state != IN_THE_BRICK)
	{
		x += vx * dt;
		y += vy * dt;
	}
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::SetState(int state)
{
	switch (state)
	{
	case OUT_OF_BRICK:
		if (mario->GetMarioPositionX() > this->x)
			vx = -MUSHROOM_SPEED_X;
		else 
			vx = MUSHROOM_SPEED_X;
		break;
	}
	//Nếu qua trái mà gần với Mario thì nấm qua phải
	CGameObject::SetState(state);
}