#include "Tail.h"
#include "Goomba.h"
#include "Mario.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render()
{
	RenderBoundingBox(); 
}

void CTail::OnNoCollision(DWORD dt)
{
	//nothing
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj))//&&isattacking)
		OnCollisionWithGoomba(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (state == TAIL_STATE_ATTACK)
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
	}
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH / 2;
	t = y - TAIL_BBOX_HEIGHT / 2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}

void CTail::SetState(int state)
{
	switch (state)
	{
	case TAIL_STATE_ATTACK:

		break;
	}
}