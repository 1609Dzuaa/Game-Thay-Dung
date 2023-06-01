#include "Mushroom.h"
#include "Mario.h"
#include "debug.h"

//extern CMario* mario;

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == IN_THE_BRICK)
		animations->Get(ID_ANI_MUSHROOM_IN_BRICK)->Render(x, y);
	else
		animations->Get(ID_ANI_MUSHROOM_OUT_BRICK)->Render(x, y);
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
		if (y <= brickminY )//&& GetTickCount64() - hitted_time >= MUSHROOM_RISEN_UP_TIME)
		{
			hitted_time = 0;
			y = brickminY;
			this->SetState(OUT_OF_BRICK);
		}
	}
	else 
	{
		vy += ay * dt;
		vx += ax * dt;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Y: %f", y);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	if (state != IN_THE_BRICK)
	{
		x += vx * dt;
		y += vy * dt;
	}
	else
	{
		y += -vy * dt;
	}
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	HandleCollisionWithBlockingObjects(e);
}

void CMushroom::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
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
		//if (this->x > mario->GetMarioPositionX())
			vx = MUSHROOM_SPEED_X;
		//else if(this->x < mario->GetMarioPositionX())
			//vx = -MUSHROOM_SPEED_X;
		break;
	}
	//Nếu qua trái mà gần với Mario thì nấm qua phải
	CGameObject::SetState(state);
}