#include "Mushroom.h"
#include "debug.h"

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
	//vy += ay * dt;
	//vx += ax * dt;
	vy += ay * dt;
	//isOutOfBrick = false;

	if (!isOutOfBrick)
	{
		if (y <= brickminY)
		{
			//vy = 0;
			y = brickminY;
			isOutOfBrick = true;
			//vx += ax * dt;
		}
		else
		{
			y -= MUSHROOM_RISE_UP_SPEED * dt;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"UNTOUCHABLE: %f, %f, %f, %f, %d", brickminY, y, vy, vx, isOutOfBrick);
	DebugOutTitle(L"IS COLLIDABLE: %d", IsCollidable());
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	//DebugOutTitle(L"KHONG CO VA CHAM");
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (untouchable != 1)
	//{
		if (!e->obj->IsBlocking()) return;

		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
		DebugOutTitle(L"VA CHAM");
	//}
}

void CMushroom::SetState(int state)
{
	switch (state)
	{
	case OUT_OF_BRICK:

		break;
	}
}