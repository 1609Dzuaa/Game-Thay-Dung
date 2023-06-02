#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0; 
	this->ay = KOOPA_GRAVITY;
	this->type = type;
	die_start = -1;
	reborn_start = -1;
	knock_off_start = -1;
	isOnPlatform = false;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Các trạng thái ở trong mai
	if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_DIE
		|| state == KOOPA_STATE_SLIP || state == KOOPA_STATE_REBORN
		|| state == KOOPA_STATE_SLEEP_REVERSE || state == KOOPA_STATE_REBORN_REVERSE
		|| state == KOOPA_STATE_SLIP_REVERSE || state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_IN_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_IN_SHELL_BBOX_HEIGHT;
	}
	else //trạng thái bình thường
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
	//DebugOutTitle(L"OCW: %f, %f, %d", vx, vy, isOnPlatform);
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{   
	//Koopa chết:
	//1.Rơi vào chế độ ngủ đông và bị Mario quăng đập vào enemy khác
	//2.Va chạm với Koopa mà bị Mario quăng
	
	KindOfCollisionWith(e);
	if (!e->obj->IsBlocking()) return;

	HandleCollisionWithBlockingObjects(e);
}

void CKoopa::KindOfCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj) && this->state == KOOPA_STATE_SLIP
		|| dynamic_cast<CGoomba*>(e->obj) && this->state == KOOPA_STATE_SLIP_REVERSE)
		this->OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj) && this->state == KOOPA_STATE_SLIP
		|| dynamic_cast<CKoopa*>(e->obj) && this->state == KOOPA_STATE_SLIP_REVERSE)
		this->OnCollisionWithKoopa(e);
	if (dynamic_cast<CQuestionBrick*>(e->obj) && this->state == KOOPA_STATE_SLIP && e->obj->GetState() != QBRICK_STATE_HITTED
		|| dynamic_cast<CQuestionBrick*>(e->obj) && this->state == KOOPA_STATE_SLIP_REVERSE && e->obj->GetState() != QBRICK_STATE_HITTED)
		this->OnCollisionWithQuesBrick(e);
}

void CKoopa::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) //Nếu object có thuộc tính block
	{
		if (type != GREEN_FLYING_KOOPA)
			vy = 0;
		else
			SetState(KOOPA_STATE_JUMPING); //Loại 2 chạm sàn thì bắt đầu nhảy

		if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_SLEEP_REVERSE)
			vx = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isOnPlatform = false;

	vx += ax * dt;
	vy += ay * dt;

	UpdateKoopaState();
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"STATE: %d", state);
}

void CKoopa::UpdateKoopaState()
{
	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	else if ((state == KOOPA_STATE_SLEEP) && (GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT))
	{
		sleep_start = 0;
		SetState(KOOPA_STATE_REBORN);
		//Hết thời gian ngủ
	}
	else if ((state == KOOPA_STATE_SLEEP_REVERSE) && (GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT)
		|| (state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL) && (GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT))
	{
		sleep_start = 0;
		SetState(KOOPA_STATE_REBORN_REVERSE);
		//Hết thời gian ngủ lật mai
	}
	else
		if ((state == KOOPA_STATE_REBORN) && (GetTickCount64() - reborn_start >= KOOPA_REBORN_TIMEOUT)
			|| (state == KOOPA_STATE_REBORN_REVERSE) && (GetTickCount64() - reborn_start >= KOOPA_REBORN_TIMEOUT))
		{
			reborn_start = 0;
			SetState(KOOPA_STATE_WALKING);
			//Hết thời gian reborn => Walking như bình thường
		}
}

void CKoopa::Render()
{
	int aniId = 0;
	
	if (type == GREEN_KOOPA)
		aniId = GetAniIdGreenKoopa();
	else if (type == GREEN_FLYING_KOOPA)
		aniId = GetAniIdFlyingKoopa();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case KOOPA_STATE_SLEEP:
		vx = 0;
		isOnPlatform = true;
		sleep_start = GetTickCount64();
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_SLEEP_REVERSE: 
		if (this->x > mario->GetMarioPositionX()) //Dựa vào vị trí để đẩy con Koopa đi xa khỏi Mario
			vx = KOOPA_KNOCK_OFF_VELO_X * KOOPA_KNOCK_OFF_FACTOR_X;
		else
			vx = -KOOPA_KNOCK_OFF_VELO_X * KOOPA_KNOCK_OFF_FACTOR_X;
		
		vy = -KOOPA_KNOCK_OFF_FACTOR_Y;	
		isOnPlatform = true;
		sleep_start = GetTickCount64();
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_SLEEP_REVERSE_SPECIAL:
		vx = 0;
		isOnPlatform = true;
		sleep_start = GetTickCount64();
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_REBORN:
		reborn_start = GetTickCount64();
		break;
		
	case KOOPA_STATE_REBORN_REVERSE:
		vx = 0;
		reborn_start = GetTickCount64();
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		isOnPlatform = true;
		break;

	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		if (this->nx > 0)
			vx = -vx * KOOPA_KNOCK_OFF_FACTOR_X;
		else
			vx = vx * KOOPA_KNOCK_OFF_FACTOR_X;
		vy = -KOOPA_KNOCK_OFF_FACTOR_Y;
		break;

	case KOOPA_STATE_SLIP:
		if (mario->GetMarioNormalX() > 0)
			vx = KOOPA_SLIPPING_SPEED;
		else 
			vx = -KOOPA_SLIPPING_SPEED;
		vy = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_SLIP_REVERSE:
		if (mario->GetMarioNormalX() > 0)
			vx = KOOPA_SLIPPING_SPEED;
		else
			vx = -KOOPA_SLIPPING_SPEED;
		vy = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_WALKING:
		if (this->x > mario->GetMarioPositionX())
			vx = -KOOPA_WALKING_SPEED;
		else 
			vx = KOOPA_WALKING_SPEED;

		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2; 
		//Cập nhật vị trí y mới sau khi từ trạng thái NGỦ đến lúc hồi sinh
		//để tránh việc bị rơi xuống nền
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
	else if (state == KOOPA_STATE_SLIP_REVERSE)
		id = ID_ANI_KOOPA_SLIPPING_REVERSE;
	else if (state == KOOPA_STATE_REBORN)
		id = ID_ANI_KOOPA_REBORN;
	else if (state == KOOPA_STATE_REBORN_REVERSE)
		id = ID_ANI_KOOPA_REBORN_REVERSE;
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