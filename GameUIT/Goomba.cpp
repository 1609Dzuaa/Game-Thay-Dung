#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "debug.h"

//extern CMario* mario;

CGoomba::CGoomba(float x, float y, int type) :CGameObject(x, y)
{
	this->vx = -GOOMBA_WALKING_SPEED;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	this->type = type;
	die_start = -1;
	die_reverse_start = -1;
	fly_start = 0;
	count_step = 0;
	count_step_to_jump = 0;
	isDead = false;
	isSpreadWings = false;
	this->SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->type == PARA_GOOMBA && this->level == PARA_GOOMBA_LEVEL_NO_WINGS || this->type != PARA_GOOMBA)
	{
		if (state == GOOMBA_STATE_DIE)
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
		}
		else
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
	}
	else //Loại 2 có cánh
	{
		if (isSpreadWings)
		{
			left = x - PARA_GOOMBA_SPREAD_WINGS_BBOX_WIDTH / 2;
			top = y - PARA_GOOMBA_SPREAD_WINGS_BBOX_HEIGHT / 2;
			right = left + PARA_GOOMBA_SPREAD_WINGS_BBOX_WIDTH;
			bottom = top + PARA_GOOMBA_SPREAD_WINGS_BBOX_HEIGHT;
		}
		else 
		{
			left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
			top = y - PARA_GOOMBA_BBOX_HEIGHT / 2;
			right = left + PARA_GOOMBA_BBOX_WIDTH;
			bottom = top + PARA_GOOMBA_BBOX_HEIGHT;
		}
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//Xét va chạm giữa Goomba và VẬT KHÁC

	if (!e->obj->IsBlocking()) return; //Nếu nó không có thuộc tính block thì kết thúc hàm này (On no collision inside here)
	if (dynamic_cast<CGoomba*>(e->obj)) return; //Nếu VẬT KHÁC LÀ Goomba thì cũng bỏ qua hàm này 

	HandleCollisionWithBlockingObjects(e);
	//DebugOutTitle(L"Coins: %f", e->ny);
}

void CGoomba::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	//Xét vật có thuộc tính BLOCK
		
	if (e->ny != 0)
	{
		if (e->ny < 0 && this->type > GOOMBA && state == GOOMBA_STATE_READY_TO_FLY)
		{
			vy = 0;
			count_step_to_jump++;
			this->SetState(GOOMBA_STATE_READY_TO_FLY);
		}
		else if (e->ny < 0 && this->type > GOOMBA)
		{
			vy = 0;
			count_step++;
		}
		else
			vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	UpdateGoombaState();
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"GOOMBA STATE AND COUNT_STEP: %d, %d, %d", state, count_step, count_step_to_jump);
}

void CGoomba::UpdateGoombaState()
{
	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		isDead = true;
		return;
	}
	else if ((state == GOOMBA_STATE_DIE_REVERSE) && (GetTickCount64() - die_reverse_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		isDead = true;
		return;
	}

	else if ((state == GOOMBA_STATE_WALKING) && count_step % 60 == 0 && count_step != 0)
	{
		count_step = 0;
		this->SetState(GOOMBA_STATE_READY_TO_FLY);
	}
	else if ((state == GOOMBA_STATE_READY_TO_FLY) && count_step_to_jump % 3 == 0 && count_step_to_jump != 0)
	{
		count_step_to_jump = 0;
		this->SetState(GOOMBA_STATE_FLYING);
	}
	else if ((state == GOOMBA_STATE_FLYING) && (GetTickCount64() - fly_start > GOOMBA_FLY_TIMEOUT))
	{
		fly_start = 0;
		this->SetState(GOOMBA_STATE_WALKING);
	}
}

void CGoomba::Render()
{
	int aniId;
	if (type == GOOMBA)
	{
		aniId = ID_ANI_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE)
			aniId = ID_ANI_GOOMBA_DIE;
		if (state == GOOMBA_STATE_DIE_REVERSE)
			aniId = ID_ANI_GOOMBA_DIE_REVERSE;
	}
	else
	{
		aniId = ID_ANI_PARA_GOOMBA_HAS_WINGS_WALKING;
		if(state == GOOMBA_STATE_WALKING && level == PARA_GOOMBA_LEVEL_NO_WINGS)
			aniId = ID_ANI_PARA_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE)
			aniId = ID_ANI_PARA_GOOMBA_DIE;
		if (state == GOOMBA_STATE_DIE_REVERSE)
			aniId = ID_ANI_PARA_GOOMBA_DIE_REVERSE;
		if (state == GOOMBA_STATE_READY_TO_FLY)
			aniId = ID_ANI_PARA_GOOMBA_READY_TO_FLY;
		if (state == GOOMBA_STATE_FLYING)
			aniId = ID_ANI_PARA_GOOMBA_FLYING;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
	//DebugOutTitle(L"Value vy: %f", vy);
}

void CGoomba::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case GOOMBA_STATE_DIE_REVERSE:
		die_reverse_start = GetTickCount64();
		if (this->x > mario->GetMarioPositionX())
			vx = abs(vx) * GOOMBA_DIE_REVERSE_FACTOR_X;
		else if (this->x <= mario->GetMarioPositionX() && this->vx < 0)
			vx = vx * GOOMBA_DIE_REVERSE_FACTOR_X;
		else
			vx = -vx * GOOMBA_DIE_REVERSE_FACTOR_X;
		vy = -GOOMBA_DIE_REVERSE_FACTOR_Y;
		break;

	case GOOMBA_STATE_WALKING:
		break;
		
	case GOOMBA_STATE_READY_TO_FLY:
		vy = -GOOMBA_READY_TO_FLY_SPEED;
		break;

	case GOOMBA_STATE_FLYING:
		fly_start = GetTickCount64();
		vy = -GOOMBA_FLYING_SPEED;
		break;
	}

	CGameObject::SetState(state);
}
