#include"KoopaPara.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "debug.h"

CKoopaPara::CKoopaPara(float x, float y) :CKoopa(x, y, type)
{
	this->ax = 0; //Chuyển động đều 
	this->ay = KOOPA_GRAVITY;
	isOnPlatform = false;
	isStepOn = false;
	SetState(KOOPA_PARA_STATE_JUMPING);
}

void CKoopaPara::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;

}

void CKoopaPara::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaPara::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj) && this->state == KOOPA_STATE_SLIP)
		this->OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj) && this->state == KOOPA_STATE_SLIP)
		this->OnCollisionWithKoopa(e);
	//if (dynamic_cast<CQuestionBrick*>(e->obj) && this->state == KOOPA_STATE_SLIP && e->obj->GetState() != QBRICK_STATE_HITTED)
		//this->OnCollisionWithQuesBrick(e);
	if (!e->obj->IsBlocking()) return;


	if (e->ny < 0) //Nếu object có thuộc tính block
	{
		isOnPlatform = true;
		SetState(KOOPA_PARA_STATE_JUMPING);
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopaPara::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	isOnPlatform = false;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"KOOPA VA CHAM: %f", vx);
}

void CKoopaPara::Render()
{
	int aniId = 0;
	if (vx < 0)
		aniId = ID_ANI_KOOPA_PARA_WALKING_LEFT;
	else
		aniId = ID_ANI_KOOPA_PARA_WALKING_RIGHT;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopaPara::SetState(int state)
{
	switch (state)
	{
	case KOOPA_PARA_STATE_JUMPING:
		vy = -KOOPA_PARA_JUMP_SPEED;
		break;
	}

	CGameObject::SetState(state);
}

void CKoopaPara::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
}

void CKoopaPara::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetState() != KOOPA_STATE_DIE)
		koopa->SetState(KOOPA_STATE_DIE);
}

/*void CKoopaPara::OnCollisionWithQuesBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
	if (qb->GetState() != QBRICK_STATE_HITTED)
		qb->SetState(QBRICK_STATE_HITTED);
}*/