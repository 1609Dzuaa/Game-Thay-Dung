#include "StarIntro.h"
#include "Card.h"
#include "debug.h"

void CStarIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	//DebugOutTitle(L"Y: %f", this->y);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CStarIntro::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CStarIntro::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		vx = 0.15f;
		vy = -STAR_INTRO_BOUNCING_SPEED;
	}
}

void CStarIntro::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_CARD_STAR)->Render(x, y, false);
}

void CStarIntro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - STAR_INTRO_BBOX_WIDTH / 2;
	t = y - STAR_INTRO_BBOX_HEIGHT / 2;
	r = l + STAR_INTRO_BBOX_WIDTH;
	b = t + STAR_INTRO_BBOX_HEIGHT;
}