#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0; //Chuyển động đều 
	this->ay = KOOPA_GRAVITY;
	this->type = type;
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
	x += vx * dt;
	y += vy * dt;
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
		if (type != 2)
			vy = 0;
		else
			SetState(KOOPA_STATE_JUMPING);

		isOnPlatform = true;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Should create a function Handle these complicated code
	if (type != 2)
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
	}
	else
	{
		vx += ax * dt;
		vy += ay * dt;
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
}


void CKoopa::Render()
{
	int aniId = 0;
	
	if (type == 1)
		aniId = GetAniIdGreenKoopa();
	else if (type == 2)
		aniId = GetAniIdFlyingKoopa();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	switch (state)
	{
	case KOOPA_STATE_SLEEP:
		vx = 0;
		isStepOn = true;
		isOnPlatform = true;
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_IN_SHELL_HEIGHT) / 2;
		//y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
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

	case KOOPA_STATE_JUMPING:
		vy = -KOOPA_JUMP_SPEED;
		break;
	}

	CGameObject::SetState(state);
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

int CKoopa::GetAniIdGreenKoopa()
{
	int id = -1;
	if (state == KOOPA_STATE_WALKING && this->vx > 0)
		id = ID_ANI_KOOPA_WALKING_RIGHT;
	else if (state == KOOPA_STATE_WALKING && this->vx < 0)
		id = ID_ANI_KOOPA_WALKING_LEFT;
	else if (state == KOOPA_STATE_SLEEP)
		id = ID_ANI_KOOPA_SLEEPING;
	else if (state == KOOPA_STATE_SLIP)
		id = ID_ANI_KOOPA_SLIPPING;
	else
		id = ID_ANI_KOOPA_DIE;

	return id;
}

int CKoopa::GetAniIdFlyingKoopa()
{
	if (vx > 0)
		return ID_ANI_KOOPA_JUMPING_RIGHT;
	else
		return ID_ANI_KOOPA_JUMPING_LEFT;
}