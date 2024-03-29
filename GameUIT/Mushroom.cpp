﻿#include "Mushroom.h"
#include "Mario.h"
#include "PlayScene.h"
#include "ColorPlatform.h"
#include "debug.h"
#include "DataBinding.h"

void CMushroom::Render()
{
	if (IsWaiting && IsWaitable) return;

	CAnimations* animations = CAnimations::GetInstance();
	if (state != MUSHROOM_STATE_IN_THE_BRICK)
	{
		if(type == RED_MUSHROOM)
			animations->Get(ID_ANI_RED_MUSHROOM)->Render(x, y, true);
		else 
			animations->Get(ID_ANI_GREEN_MUSHROOM)->Render(x, y, true);
	}
	//Out of brick & rise up share the same animation
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
	if (CDataBindings::GetInstance()->IsStopWatch) 
		return;
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();


    if (state != MUSHROOM_STATE_OUT_OF_BRICK && current_scene->GetID() == ID_MAP_1_1)
	{
		if (y <= brickminY )
		{
			y = brickminY;
			this->SetState(MUSHROOM_STATE_OUT_OF_BRICK);
		}
	}
	else 
	{
		vy += ay * dt;
		vx += ax * dt;
	}
	//DebugOutTitle(L"Vx: %f", vx);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	if (state != MUSHROOM_STATE_RISE_UP)
	{
		x += vx * dt;
		y += vy * dt;
	}
	else
	{
		y += vy * dt;
	}
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CColorPlatform*>(e->obj))
	{
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
	}
	else if (!e->obj->IsBlocking())
		return;

	HandleCollisionWithBlockingObjects(e);
}

void CMushroom::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	if (MUSHROOM_BBOX_HEIGHT + this->y > color_platf->GetY())
	{
		this->y = (color_platf->GetY() - MUSHROOM_BBOX_HEIGHT);
		vy = 0;
	}
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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_RISE_UP:
		this->vy = -MUSHROOM_RISE_UP_SPEED;
		break;

	case MUSHROOM_STATE_OUT_OF_BRICK:
		if (this->x > mario->GetX())
			vx = MUSHROOM_SPEED_X;
		else if(this->x<mario->GetX())
			vx = -MUSHROOM_SPEED_X;
		break;
	}
	//Để nấm lúc chui ra thì tiến ra xa khỏi Mario
	CGameObject::SetState(state);
}