#include"Koopa.h"
#include "Goomba.h"
#include "Mario.h"
#include "MarioNPC.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "EffectScore.h"
#include "EffectCollision.h"
#include "Flower.h"
#include "debug.h"
#include "DataBinding.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->type = type;
	if (type == RED_KOOPA)
	{
		SetState(KOOPA_STATE_WALKING);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		if (vx < 0)
			ghost_head = new CHead(x - KOOPA_BBOX_WIDTH / 2 - 2.5f, y, this->vx, this->ay);
		else if (vx > 0)
			ghost_head = new CHead(x + KOOPA_BBOX_WIDTH / 2 + 2.5f, y, this->vx, this->ay);
		current_scene->AddObjectToScene(ghost_head);
		enableToChangeVx = true; //mấy thuộc tính của Red Koopa nên cân nhắc khởi tạo hợp lý
		DebugOut(L"Red Koopa's Ghost Head was created\n");
	}
	else if (type == GREEN_FLYING_KOOPA)
	{
		SetState(KOOPA_STATE_JUMPING);
		enableToChangeVx = false;
	}
	else //loại thường
	{
		SetState(KOOPA_STATE_WALKING);
		enableToChangeVx = false;
	}
	die_start = -1;
	sleep_start = -1;
	reborn_start = -1;
	knock_off_start = -1;
	KillCount = 0;
	isOnPlatform = false;
	isBeingHeld = false;
	IsWaitable = true;
	IsWaiting = false;
	isGreen = 0;
	isBlack = 0;
}

CKoopa::CKoopa(float x, float y, int isGreen, int isBlack) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->type = GREEN_KOOPA;
	this->isGreen = isGreen;
	this->isBlack = isBlack;
	ghost_head = nullptr;
	enableToChangeVx = false;
	die_start = -1;
	sleep_start = -1;
	reborn_start = -1;
	knock_off_start = -1;
	KillCount = 0;
	isOnPlatform = false;
	isBeingHeld = false;
	IsWaitable = true;
	IsWaiting = false;
	SetState(KOOPA_STATE_SLEEP);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Các trạng thái ở trong mai
	if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_DIE
		|| state == KOOPA_STATE_SLIP || state == KOOPA_STATE_REBORN
		|| state == KOOPA_STATE_SLEEP_REVERSE || state == KOOPA_STATE_REBORN_REVERSE
		|| state == KOOPA_STATE_SLIP_REVERSE || state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL
		|| state == KOOPA_STATE_BOUNCING || state == KOOPA_STATE_BEING_HELD)
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
	//DebugOut(L"ONC: %f, %f, %d", vx, vy, isOnPlatform);
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//Koopa chết:
	//1.Rơi vào chế độ ngủ đông và bị Mario quăng đập vào enemy khác
	//2.Va chạm với Koopa mà bị Mario quăng

	KindOfCollisionWith(e);
	if (!e->obj->IsBlocking())
			return;

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
	if (dynamic_cast<CFlower*>(e->obj))
		this->OnCollisionWithFlower(e);
}

void CKoopa::HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (isBlack && countBounce < 2)
	{
		if (e->ny < 0)
			SetState(KOOPA_STATE_BOUNCING);
		return;
	}
	else if (isBlack)
	{
		vy = 0;
		isOnPlatform = true;
		return;
	}

	if (e->ny == -1) //Giải quyết đc tình trạng Flyin Koopa đập đầu vào thì không nhảy nữa
	{
		if (state == KOOPA_STATE_SLEEP_REVERSE)
			state = KOOPA_STATE_SLEEP_REVERSE_SPECIAL;
		if (type != GREEN_FLYING_KOOPA)
		{
			vy = 0;
			isOnPlatform = true;
		}
		else if (type == GREEN_FLYING_KOOPA)
			this->SetState(KOOPA_STATE_JUMPING);

		if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
			vx = 0;
	}
	else if (e->ny == 1)
	{
		if (type == GREEN_FLYING_KOOPA)
			SetState(KOOPA_STATE_JUMPING); //Loại 2 chạm sàn thì bắt đầu nhảy
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		//Nếu đang trượt mà đụng phải GOLD_BRICK -> xoá nó
		if (dynamic_cast<CBrick*>(e->obj))
		{
			CBrick* br = dynamic_cast<CBrick*>(e->obj);
			if (br->GetType() == GOLD_BRICK)
			{
				if (br->GetItemType() == NO_ITEM)
				{
					if (this->state == KOOPA_STATE_SLIP || state == KOOPA_STATE_SLIP_REVERSE)
						br->SetState(GBRICK_STATE_IS_DESTROYED);
				}
				else
				{
					if (this->state == KOOPA_STATE_SLIP || state == KOOPA_STATE_SLIP_REVERSE)
					{
						br->SetSpeed(0, -GOLD_BRICK_BOUNCING_SPEED);
						br->SetHitted(true);
						br->SetState(GBRICK_HAS_ITEM_STATE_IS_HITTED);
					}
				}

			}
		}

		if (type == RED_KOOPA)
		{
			if (vx > 0)
			{
				ghost_head->SetSpeed(this->vx, 0);
				ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 5.0f, y);
			}
			else
			{
				ghost_head->SetSpeed(this->vx, 0);
				ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, y);
			}
		}
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	if (!CCamera::GetInstance()->isViewable(this)
		&& current_scene->GetID() == ID_MAP_1_1)
		return;
	
	//if (IsWaiting && IsWaitable)
		//;//giữ nguyên thời gian khi đang wait

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	if (CDataBindings::GetInstance()->IsStopWatch) 
		return;

	//TH Bị cầm ở Intro thì có hàm Update vị trí ở Luigi
	if (current_scene->GetID() == ID_MAP_1_1)
	{
		if (isBeingHeld)
		{
			vx = vy = 0;
			if (mario->GetMarioNormalX() > 0)
			{
				if (mario->GetLevel() > MARIO_LEVEL_SMALL)
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
	}
	isOnPlatform = false;

	vx += ax * dt;
	vy += ay * dt;

	UpdateKoopaState();
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"ay, st, COLLIDABLE: %f, %d, %d", ay, state, IsCollidable());
}

void CKoopa::UpdateKoopaState()
{
	if (isGreen || isBlack) 
		return; //0 Update State Cho 2 con Koopa đầu Intro

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CPlayScene* play_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	//Quay đầu của Red Koopa
	if (type == RED_KOOPA)
	{
		//chỉ khi đc cho phép tương tác thay đổi vx trên vật đặc biệt
		//Tránh việc đang slip mà vẫn thay đổi vx
		if (state != KOOPA_STATE_SLIP && state != KOOPA_STATE_SLIP_REVERSE)
			enableToChangeVx = true;
		else
			enableToChangeVx = false;

		if (enableToChangeVx)
		{
			if (ghost_head->GetY() - this->y > FALL_ZONE && state != KOOPA_STATE_SLEEP_REVERSE)
			{
				this->vx = -vx;
				ghost_head->SetSpeed(vx, 0);
				if (this->vx < 0)
					ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, this->y);
				else
					ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 5.0f, this->y);
			}
			//Update lại vị trí cái đầu nếu đi quá FALL_ZONE
		}
	}

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		if (type == RED_KOOPA)
			ghost_head->Delete(); //consider remove it out of vector objects ?
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
			if (isBeingHeld) 
			{
				mario->SetIsHoldingKoopa(false);
				if (mario->GetLevel() > 2)
				{
					mario->SetState(MARIO_STATE_EVOLVING);
					mario->SetEvolvingBackWard(true);
					CEffectCollision* eff = new CEffectCollision(x, y, GetTickCount64(), EFF_COL_TYPE_SMOKE_EVOLVE);
					play_scene->AddObjectToScene(eff);
					CDataBindings::GetInstance()->IsStopWatch = 1;
				}
				else if (mario->GetLevel() > 1)
				{
					mario->SetState(MARIO_STATE_EVOLVING);
					mario->SetEvolvingBackWard(true);
					CDataBindings::GetInstance()->IsStopWatch = 1;
				}
				else
					mario->SetState(MARIO_STATE_DIE);
			}
			SetState(KOOPA_STATE_WALKING);
			isBeingHeld = false;
			//Hết thời gian reborn => Walking như bình thường
			//Set lại vị trí cũng như vận tốc cho cái đầu nếu là Red koopa
		}
}

void CKoopa::Render()
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	if (!CCamera::GetInstance()->isViewable(this)
		&& current_scene->GetID() == ID_MAP_1_1)
		return;

	if (IsWaiting && IsWaitable) 
		return;
	
	//RenderBoundingBox();
	//Nếu đang bị hold và chưa tới thời gian reborn
	//if (isBeingHeld) return; //how 'bout a litte trick here ?

	int aniId = 0;

	if (type == GREEN_KOOPA)
		aniId = GetAniIdGreenKoopa();
	else if (type == GREEN_FLYING_KOOPA)
		aniId = GetAniIdFlyingKoopa();
	else
		aniId = GetAniIdRedKoopa();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y, true);
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
			{
				if (vx < 0)
					ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, y); //cập nhật vị trí cho cái đầu để tránh nó đi quá xa
				else 
					ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 5.0f, y); //cập nhật vị trí cho cái đầu để tránh nó đi quá xa
			}
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
		ay = KOOPA_GRAVITY;
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
	{	
		CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
		ay = KOOPA_GRAVITY; //trả lại trọng lực cho nó

		if (current_scene->GetID() == ID_MAP_1_1)
		{
			if (mario->GetMarioNormalX() > 0)
				vx = KOOPA_SLIPPING_SPEED;
			else
				vx = -KOOPA_SLIPPING_SPEED;
		}
		else //Intro
		{
			if (nxNPC > 0)
				vx = 0.1f;
			else 
				vx = -0.1f;
		}
		vy = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		break;

    }

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
		ay = KOOPA_GRAVITY; //Quên chưa trả :v
		if (mario == NULL) vx = KOOPA_WALKING_SPEED; //nếu ban đầu chưa có mario thì set v dương cho nó
		else
		{
			if (this->x > mario->GetX())
			{
				vx = -KOOPA_WALKING_SPEED;
				if (type == RED_KOOPA && ghost_head != NULL)
				{
					ghost_head->SetPosition(x - KOOPA_BBOX_WIDTH / 2 - 5.0f, y);
					ghost_head->SetSpeed(this->vx, 0);
				}
			}
			else
			{
				vx = KOOPA_WALKING_SPEED;
				if (type == RED_KOOPA && ghost_head != NULL)
				{
					ghost_head->SetSpeed(this->vx, 0);
					ghost_head->SetPosition(x + KOOPA_BBOX_WIDTH / 2 + 5.0f, y);
				}
			}
		}
		//Cập nhật lại vị trí ghost_head nếu là Red Koopa

		y -= (KOOPA_BBOX_HEIGHT - KOOPA_IN_SHELL_BBOX_HEIGHT) / 2;
		//Cập nhật vị trí y mới sau khi từ trạng thái NGỦ đến lúc hồi sinh
		//để tránh việc bị rơi xuống nền
		break;

	case KOOPA_STATE_JUMPING:
		vy = -KOOPA_JUMP_SPEED;
		vx = -KOOPA_WALKING_SPEED;
		break;

	case KOOPA_STATE_BEING_HELD:
		ay = 0;
		vy = 0;
		vx = 0;
		isBeingHeld = true;
		break;

	case KOOPA_STATE_BEING_HELD_REVERSE:
		ay = 0;
		vy = 0;
		vx = 0;
		isBeingHeld = true;

		break;

	case KOOPA_STATE_BOUNCING:
		if (countBounce == 0)
		{
			vy = -0.3f;
			countBounce++;
		}
		else if (countBounce)
		{
			vy = -0.15f;
			countBounce++;
		}
		else vy = 0.0f;

		break;
	}

	CGameObject::SetState(state);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (isGreen) return; //Koopa ở Intro 0 va chạm với Goomba

	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	KillCount++;
	if (goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
	{
		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
		mario->SpawnScore(goomba, 1, KillCount);
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
		//mario->SpawnScore(koopa);
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
	if (e->obj->GetState() != FLOWER_STATE_DIE)
		e->obj->SetState(FLOWER_STATE_DIE);
}

int CKoopa::GetAniIdGreenKoopa()
{
	int id = -1;

	//For black Koopa at Intro Only
	if (isBlack)
	{
		if (state != KOOPA_STATE_DIE)
			return ID_ANI_BLACK_KOOPA_SHELL;
		else
			return ID_ANI_BLACK_KOOPA_DIE;
	}

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

	if (state == KOOPA_STATE_SLEEP_REVERSE && this->type == RED_KOOPA)
		this->SetState(KOOPA_STATE_SLEEP_REVERSE_SPECIAL);

	if (this->type == GREEN_FLYING_KOOPA)
		SetState(KOOPA_STATE_JUMPING);

	AdjustPositionOnColorPlatform(color_platf);
}

void CKoopa::AdjustPositionOnColorPlatform(CColorPlatform* color_platf)
{
	//Điều chỉnh vị trí để tránh bị rơi khỏi ColorPlat
	if (this->state == KOOPA_STATE_JUMPING) return;

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