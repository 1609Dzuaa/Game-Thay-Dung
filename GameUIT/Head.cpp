﻿#include "Head.h"
#include "Brick.h"
#include "PlayScene.h"
#include "Camera.h"
#include "debug.h"
#include "DataBinding.h"

void CHead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CDataBindings::GetInstance()->IsStopWatch) return;

	if (!CCamera::GetInstance()->isViewable(this)) return;

	vy += ay * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CHead::Render()
{
	//RenderBoundingBox();
}

void CHead::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CHead::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CColorPlatform*>(e->obj))
	{
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
	}

	if (e->obj->IsBlocking()) HandleCollisionWithBlockingObjects(e);
}

void CHead::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		if (dynamic_cast<CBrick*>(e->obj))
		{
			CBrick* br = dynamic_cast<CBrick*>(e->obj);
			if (br->GetType() == GOLD_BRICK)
				vy = 0;
			else
				vy = 0;
		}
	}
	else if (e->nx != 0)
		vx = 0; //chờ đc set lại vtoc
}

void CHead::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	if (this->y + HEAD_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
	{
		this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - static_cast<float>(HEAD_BBOX_HEIGHT / 2);
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

void CHead::SetState(int state)
{
	switch (state)
	{
	case HEAD_STATE_MOVE:

		break;

	case HEAD_STATE_IDLE:
		this->vx = 0;
		this->vy=0;

		break;
	}
}