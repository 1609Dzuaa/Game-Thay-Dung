#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_DIE)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{   
	//Koopa chết:
	//1.Rơi vào chế độ ngủ đông và bị Mario quăng đập vào enemy khác
	//2.Va chạm với Koopa mà bị Mario quăng
	if (dynamic_cast<CGoomba*>(e->obj) && this->state == KOOPA_STATE_SLIP)
		this->OnCollisionWithGoomba(e);
	//if (dynamic_cast<CKoopa*>(e->obj))
		//this->OnCollisionWithKoopa(e); //Hãy kiểm tra Koopa chạm Koopa
	if (!e->obj->IsBlocking()) return;


	if (e->ny != 0) //Nếu object có thuộc tính block
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Koopa acceleration x: %f", this->ax);
}


void CKoopa::Render()
{
	int aniId = 0;
	if (vx < 0)
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
	else
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	if (state == KOOPA_STATE_SLEEP)
	{
		aniId = ID_ANI_KOOPA_SLEEPING;
	}
	if (state == KOOPA_STATE_SLIP)
	{
		aniId = ID_ANI_KOOPA_SLIPPING;
	}
	if (state == KOOPA_STATE_DIE)
	{
		aniId = ID_ANI_KOOPA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_SLEEP:
		vx = 0;
		vy = 0;
		//y = y + 2.50f;
		break;
	/*case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;*/
	case KOOPA_STATE_SLIP:
		//vx = KOOPA_WALKING_SPEED * 4;
		//ay = KOOPA_GRAVITY;
		//ax = 0.00005f;
		break;
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	float goombaVx = 0, goombaVy = 0;
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->GetSpeed(goombaVx, goombaVy);
	goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
	//if (e->nx > 0)
		//goomba->SetSpeed(-goombaVx, goombaVy);
	//else if (e->ny > 0)
		//goomba->SetSpeed(goombaVx, -goombaVy);
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	koopa->SetState(KOOPA_STATE_DIE);
}