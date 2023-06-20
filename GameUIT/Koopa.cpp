#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "EffectScore.h"
#include "EffectCollision.h"
#include "ShootingFlower.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->type = type;
	SetState(KOOPA_STATE_WALKING);

	die_start = -1;
	sleep_start = -1;
	reborn_start = -1;
	knock_off_start = -1;
	KillCount = 0;
	isOnPlatform = false;
	isFallOffColorPlatform = false; //ban đầu thì Red koopa chưa rơi khỏi color plat
	enableInteractWColorPlat = true; //mấy thuộc tính của Red Koopa nên cân nhắc khởi tạo hợp lý
	isBeingHeld = false;
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
	if (dynamic_cast<CColorPlatform*>(e->obj))
	{
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
	}
	if (dynamic_cast<CQuestionBrick*>(e->obj) && e->obj->GetState() != QBRICK_STATE_HITTED
		|| dynamic_cast<CQuestionBrick*>(e->obj) && e->obj->GetState() != QBRICK_STATE_HITTED)
		this->OnCollisionWithQuesBrick(e);

	if (!ConditionsThatEnableToKillAllies()) return; //3 thằng còn lại là Enemies rồi nên đặt đây là hợp lý

	if (dynamic_cast<CGoomba*>(e->obj))
		this->OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		this->OnCollisionWithKoopa(e);
	if (dynamic_cast<CShootingFlower*>(e->obj))
		this->OnCollisionWithFlower(e);
}

void CKoopa::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) //Nếu object có thuộc tính block
	{
		if (type == RED_KOOPA)
		{
			enableInteractWColorPlat = false; //không cho tương tác với color plat một khi va chạm với blocking obj
			isFallOffColorPlatform = true; //đánh dấu đã bị rơi xuống nền
			if (state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
				state = KOOPA_STATE_SLEEP_REVERSE;
		}
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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetStopWatch()) return;

	if (!mario->GetIsHolding() && StateThatEnableToRelease() && isBeingHeld)
		HandleReleaseKoopa(); //thả Koopa khi ngưng giữ nút A

	if (isBeingHeld)
	{
		if (mario->GetMarioNormalX() > 0)
		{
			if(mario->GetLevel() > MARIO_LEVEL_SMALL)
				this->SetPosition(mario->GetX() + 11.5f, mario->GetY() + 1.5f);
			else 
				this->SetPosition(mario->GetX() + 11.5f, mario->GetY() - 1.5f);
		}
		else
		{
			if (mario->GetLevel() > MARIO_LEVEL_SMALL)
				this->SetPosition(mario->GetX() - 11.5f, mario->GetY() + 1.5f);
			else
				this->SetPosition(mario->GetX() - 11.5f, mario->GetY() - 1.5f);
		}
	}
	isOnPlatform = false;

	vx += ax * dt;
	vy += ay * dt;

	UpdateKoopaState();
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::UpdateKoopaState()
{
	//Quay đầu 
	if (type == RED_KOOPA)
	{
		if (enableInteractWColorPlat) //chỉ khi đc cho phép tương tác thay đổi vx trên ColorPlat
		{
			if (ghost_head != NULL)
			{
				if (ghost_head->GetY() - this->y > FALL_ZONE && !isFallOffColorPlatform && state != KOOPA_STATE_SLEEP_REVERSE) //đầu ma rơi xuống platform 
				{
					this->vx = -vx;
					ghost_head->SetSpeed(vx, 0);
					ghost_head->SetIsFallOff(false);
					if (this->vx < 0)
						ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 10.0f, this->y);
					else
						ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 3.0f, this->y);
				}
			}
		}
	}

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		if (type == RED_KOOPA)
			delete ghost_head; //consider remove it out of vector objects ?
		return;
	}
	else if (state == KOOPA_STATE_SLEEP && GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT
		|| state == KOOPA_STATE_BEING_HELD && GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT)
	{
		sleep_start = 0;
		SetState(KOOPA_STATE_REBORN);
		//Hết thời gian ngủ
	}
	else if ((state == KOOPA_STATE_SLEEP_REVERSE) && (GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT)
		|| (state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL) && (GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT)
		|| state == KOOPA_STATE_BEING_HELD_REVERSE && GetTickCount64() - sleep_start >= KOOPA_SLEEP_TIMEOUT)
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

			if (type == RED_KOOPA)
			{
				if (ghost_head != NULL)
				{
					if (this->vx < 0)
					{
						ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, this->y);
						ghost_head->SetSpeed(-KOOPA_WALKING_SPEED, 0);
					}
					else
					{
						ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 5.0f, this->y);
						ghost_head->SetSpeed(KOOPA_WALKING_SPEED, 0);
					}
				}
			}
			//Hết thời gian reborn => Walking như bình thường
			//Set lại vị trí cũng như vận tốc cho cái đầu nếu là Red koopa
		}
}

void CKoopa::Render()
{
	//if (this->isBeingHeld) return;
	int aniId = 0;

	if (type == GREEN_KOOPA)
		aniId = GetAniIdGreenKoopa();
	else if (type == GREEN_FLYING_KOOPA)
		aniId = GetAniIdFlyingKoopa();
	else
		aniId = GetAniIdRedKoopa();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y, true);
	//RenderBoundingBox();
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
		if (this->x > mario->GetX()) //Dựa vào vị trí để đẩy con Koopa đi xa khỏi Mario
			vx = KOOPA_KNOCK_OFF_VELO_X * KOOPA_KNOCK_OFF_FACTOR_X;
		else
			vx = -KOOPA_KNOCK_OFF_VELO_X * KOOPA_KNOCK_OFF_FACTOR_X;

		if (type == RED_KOOPA)
			if (ghost_head != NULL)
				ghost_head->SetState(HEAD_STATE_IDLE);
		vy = -KOOPA_KNOCK_OFF_FACTOR_Y;
		isOnPlatform = true;
		sleep_start = GetTickCount64();
		//y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_SLEEP_REVERSE_SPECIAL:
		vx = 0;
		isOnPlatform = true;
		sleep_start = GetTickCount64();
		if (type == RED_KOOPA)
			if (ghost_head != NULL)
				ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, y); //cập nhật vị trí cho cái đầu để tránh nó đi quá xa
		//y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
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
		if (!isBeingHeld)
		{
			if (this->nx > 0)
				vx = -vx * KOOPA_KNOCK_OFF_FACTOR_X;
			else
				vx = vx * KOOPA_KNOCK_OFF_FACTOR_X;
		}
		vy = -KOOPA_KNOCK_OFF_FACTOR_Y;
		break;

	case KOOPA_STATE_SLIP:
		ay = KOOPA_GRAVITY; //trả lại trọng lực cho nó
		if (mario->GetMarioNormalX() > 0)
			vx = KOOPA_SLIPPING_SPEED;
		else
			vx = -KOOPA_SLIPPING_SPEED;
		vy = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_SLIP_REVERSE:
		ay = KOOPA_GRAVITY;
		if (mario->GetMarioNormalX() > 0)
			vx = KOOPA_SLIPPING_SPEED;
		else
			vx = -KOOPA_SLIPPING_SPEED;
		vy = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

	case KOOPA_STATE_WALKING:
		if (mario == NULL) vx = -KOOPA_WALKING_SPEED;
		else
		{
			if (this->x > mario->GetX())
				vx = -KOOPA_WALKING_SPEED;
			else
				vx = KOOPA_WALKING_SPEED;
		}

		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		//Cập nhật vị trí y mới sau khi từ trạng thái NGỦ đến lúc hồi sinh
		//để tránh việc bị rơi xuống nền
		break;

	case KOOPA_STATE_JUMPING:
		vy = -KOOPA_JUMP_SPEED;
		break;

	case KOOPA_STATE_BEING_HELD:
		ay = 0;
		//vy = 0;
		isBeingHeld = true;

	case KOOPA_STATE_BEING_HELD_REVERSE:
		ay = 0;
		//vy = 0;
		isBeingHeld = true;

		break;
	}

	CGameObject::SetState(state);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	KillCount++;
	if (goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
	{
		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
		mario->SpawnScore(goomba);
	}
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	KillCount++;
	if (koopa->GetState() != KOOPA_STATE_DIE)
	{
		koopa->SetState(KOOPA_STATE_DIE);
		mario->SpawnScore(koopa);
	}
}

void CKoopa::OnCollisionWithQuesBrick(LPCOLLISIONEVENT e)
{
	if (state != KOOPA_STATE_SLIP && state != KOOPA_STATE_SLIP_REVERSE) return;

	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
	//if (!isBeingHeld)
	//{
		if (qb->GetState() != QBRICK_STATE_HITTED)
			qb->SetState(QBRICK_STATE_HITTED);
	//}
	//else
		//this->SetState(GOOMBA_STATE_DIE_REVERSE);
    //Will check this part later!
}

void CKoopa::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() != SHOOTING_FLOWER_STATE_DIE)
		e->obj->SetState(SHOOTING_FLOWER_STATE_DIE);
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
	else if (state == KOOPA_STATE_BEING_HELD)
		id = ID_ANI_KOOPA_SLEEPING;
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

int CKoopa::GetAniIdRedKoopa()
{
	int id = -1;
	if (state == KOOPA_STATE_WALKING && this->vx > 0)
		id = ID_ANI_RED_KOOPA_WALKING_RIGHT;
	else if (state == KOOPA_STATE_WALKING && this->vx < 0)
		id = ID_ANI_RED_KOOPA_WALKING_LEFT;
	else if (state == KOOPA_STATE_SLEEP)
		id = ID_ANI_RED_KOOPA_SLEEPING;
	else if (state == KOOPA_STATE_SLEEP_REVERSE || state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
		id = ID_ANI_RED_KOOPA_SLEEPING_REVERSE;
	else if (state == KOOPA_STATE_SLIP)
		id = ID_ANI_RED_KOOPA_SLIPPING;
	else if (state == KOOPA_STATE_SLIP_REVERSE)
		id = ID_ANI_RED_KOOPA_SLIPPING_REVERSE;
	else if (state == KOOPA_STATE_REBORN)
		id = ID_ANI_RED_KOOPA_REBORN;
	else if (state == KOOPA_STATE_REBORN_REVERSE)
		id = ID_ANI_RED_KOOPA_REBORN_REVERSE;
	else if (state == KOOPA_STATE_BEING_HELD)
		id = ID_ANI_RED_KOOPA_SLEEPING;
	else 
		id = ID_ANI_RED_KOOPA_DIE; //watch here later

	return id;
}

void CKoopa::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	if (e->ny != -1) return; //quên mất phần này, chỉ cho phép va chạm hướng trên =]]]

	if (type == RED_KOOPA && ghost_head == NULL)
	{
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		ghost_head = new CHead(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, y, this->vx, this->ay);
		current_scene->AddObjectToScene(ghost_head);
		DebugOut(L"Head was created\n");
	}

	isFallOffColorPlatform = false; //turn out the problem here @@ fack myself

	if (state == KOOPA_STATE_SLEEP_REVERSE && this->type == RED_KOOPA)
	{
		this->SetState(KOOPA_STATE_SLEEP_REVERSE_SPECIAL);
	}

	if (state == KOOPA_STATE_WALKING) //đi bộ thì mới cho đổi dấu
		enableInteractWColorPlat = true; //cho phép tương tác với ColorPlat(đổi dấu vx)
	else
		enableInteractWColorPlat = false; //cho phép nó trượt trên color plat nhưng 0 đổi dấu đc

	if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_SLIP || state == KOOPA_STATE_REBORN
		|| state == KOOPA_STATE_SLEEP_REVERSE || state == KOOPA_STATE_REBORN_REVERSE
		|| state == KOOPA_STATE_SLIP_REVERSE || state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
	{
		if (this->y + KOOPA_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
		{
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - KOOPA_BBOX_HEIGHT / 2 + 2.5f;
			vy = 0;
		}
	}
	else
	{
		if (this->y + static_cast<float>(KOOPA_BBOX_HEIGHT / 2) + static_cast<float>(color_platf->GetCellHeight() / 2) + 1.5f > color_platf->GetY())
		{
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - static_cast<float>(KOOPA_BBOX_HEIGHT / 2) - 1.5f;
			vy = 0;
		}
	}
}

void CKoopa::HandleReleaseKoopa()
{
	isBeingHeld = false;
	if (state == KOOPA_STATE_BEING_HELD || state == KOOPA_STATE_REBORN)
		this->SetState(KOOPA_STATE_SLIP);
	else if (state == KOOPA_STATE_SLEEP_REVERSE || state == KOOPA_STATE_REBORN_REVERSE)
		this->SetState(KOOPA_STATE_SLIP_REVERSE);
}

int CKoopa::StateThatEnableToRelease()
{
	return (state == KOOPA_STATE_BEING_HELD || state == KOOPA_STATE_BEING_HELD_REVERSE || state == KOOPA_STATE_REBORN || state == KOOPA_STATE_REBORN_REVERSE);
	//Hàm này trả về các trạng thái của Koopa mà cho phép được Mario thả
}

int CKoopa::ConditionsThatEnableToKillAllies()
{
	return (state == KOOPA_STATE_SLIP || state == KOOPA_STATE_SLIP_REVERSE);
	//Trả về các điều kiện cho phép Koopa giết các đồng minh của nó
}