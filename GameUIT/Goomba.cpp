#include "Goomba.h"
#include "debug.h"

CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	die_reverse_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	//0 có va chạm thì di chuyển như thường
	//x = x0 + vt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//Xét va chạm giữa Goomba và VẬT KHÁC

	if (!e->obj->IsBlocking()) return; //Nếu nó không có thuộc tính block thì kết thúc hàm này (On no collision inside here)
	if (dynamic_cast<CGoomba*>(e->obj)) return; //Nếu VẬT KHÁC LÀ Goomba thì cũng bỏ qua hàm này 

	//Xét vật có thuộc tính BLOCK
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	//DebugOutTitle(L"Coins: %f", e->ny);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	//v=v0+at

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	else if ((state == GOOMBA_STATE_DIE_REVERSE) && (GetTickCount64() - die_reverse_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	//DebugOutTitle(L"GOOMBA STATE: %d", state);

}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
		aniId = ID_ANI_GOOMBA_DIE;
	if (state == GOOMBA_STATE_DIE_REVERSE)
		aniId = ID_ANI_GOOMBA_DIE_REVERSE;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
	//DebugOutTitle(L"Value vy: %f", vy);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	//if (state == GOOMBA_STATE_DIE || GOOMBA_STATE_DIE_REVERSE)
		//return;
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
		if (this->nx > 0)
			vx = -vx * 1.15;
		else
			vx = vx * 1.15;
		vy = -0.5f;
		break;

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}
