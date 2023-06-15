#include "Head.h"
#include "debug.h"

void CHead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	//DebugOut(L"x, y: %f, %f", x, y);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CHead::Render()
{
	
	CAnimations::GetInstance()->Get(5000)->Render(x, y);
	RenderBoundingBox();
}

void CHead::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CHead::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny < 0)
		{
			vy = 0;
			isFallOff = true;
			return;
		}
	}

	if (dynamic_cast<CColorPlatform*>(e->obj))
	{
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
	}
}

void CHead::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	if (HEAD_BBOX_HEIGHT + 3.0f + this->y > color_platf->GetY())
	{
		this->y = (color_platf->GetY() - HEAD_BBOX_HEIGHT + 3);
		vy = 0;
	}
}

void CHead::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - HEAD_BBOX_WIDTH / 2;
	t = y - HEAD_BBOX_HEIGHT / 2;
	r = l + HEAD_BBOX_WIDTH;
	b = t + HEAD_BBOX_HEIGHT;
}