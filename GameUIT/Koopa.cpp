#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0; //Chuyển động đều 
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	isOnPlatform = false;
	isStepOn = false;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_DIE || state == KOOPA_STATE_SLIP)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_IN_SHELL_HEIGHT;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_IN_SHELL_HEIGHT;
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
	if (!isOnPlatform)
	{
		x += vx * dt;
		y += vy * dt;
	}
	else 
	{
		x += vx * dt;
	}

	DebugOut(L"KOOPA KHONG VA CHAM: %d\n", isOnPlatform);
	//Hãy làm sao để mà khi con Koopa nó ngủ thì vẫn hiện On Collision With
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{   
	//Koopa chết:
	//1.Rơi vào chế độ ngủ đông và bị Mario quăng đập vào enemy khác
	//2.Va chạm với Koopa mà bị Mario quăng
	

	if (dynamic_cast<CGoomba*>(e->obj) && this->state == KOOPA_STATE_SLIP)
		this->OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj) && this->state == KOOPA_STATE_SLIP)
		this->OnCollisionWithKoopa(e);
	if (dynamic_cast<CQuestionBrick*>(e->obj) && this->state == KOOPA_STATE_SLIP && e->obj->GetState() != QBRICK_STATE_HITTED)
		this->OnCollisionWithQuesBrick(e);
	if (!e->obj->IsBlocking()) return;


	if (e->ny < 0) //Nếu object có thuộc tính block
	{
		vy = 0;
		isOnPlatform = true;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	//DebugOutTitle(L"KOOPA VA CHAM: %f\n", ay);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (!isStepOn)
	{
		vy += ay * dt;
		vx += ax * dt;
	}
	else
	{
		vx += ax * dt;
	}

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	isOnPlatform = false;
	isStepOn = false;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//Gọi liên tục

	//DebugOutTitle(L"KOOPA: %f, %f, %f", vx, vy, ay);
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
		isStepOn = true;
		isOnPlatform = true;
		break;

	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		if (this->nx > 0)
			vx = -vx * 1.15;
		else
			vx = vx * 1.15;
		vy = -0.5f;
		break;

	case KOOPA_STATE_SLIP:
		isStepOn = true;
		ay = KOOPA_GRAVITY;
		break;

	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetState() != KOOPA_STATE_DIE)
		koopa->SetState(KOOPA_STATE_DIE);
}

void CKoopa::OnCollisionWithQuesBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
	if (qb->GetState() != QBRICK_STATE_HITTED)
		qb->SetState(QBRICK_STATE_HITTED);
}