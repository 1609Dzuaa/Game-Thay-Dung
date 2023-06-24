#include "Brick.h"
#include "Mario.h"

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (isBeingHitted) ay = GOLD_BRICK_GRAVITY;
	//vy += ay * dt;

	if (y < min_pos)
	{
		vy = GOLD_BRICK_BOUNCING_SPEED;
	}
	if (y > old_pos)
	{
		y = old_pos;
		vy = 0;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrick::OnNoCollision(DWORD dt)
{
	if (type != GOLD_BRICK) return;
	if (!isBeingHitted) return;

	y += vy * dt;
}

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (type == STRIPE_BRICK)
		animations->Get(ID_ANI_BRICK)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_GOLD_BRICK)->Render(x, y, false);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}