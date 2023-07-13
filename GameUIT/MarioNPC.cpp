#include "MarioNPC.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "Leaf.h"
#include "EffectCollision.h"
#include "IntroPlayScene.h"
#include "debug.h"

void CMarioNPC::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEvolving)
	{
		vy += ay * dt;
		vx += ax * dt;
	}
	else
	{
		vx = 0;
		vy = 0;
	}

	//bị hit, nhìn lên
	if (GetTickCount64() - hitted_timeout > 900 && isHitted)
		SetState(MARIO_STATE_LOOKUP);

	//nhảy lên ăn lá
	if (GetTickCount64() - look_timeout > 900 && isLookUp)
		SetState(MARIO_STATE_JUMPING);

	//Hạ cánh giết Goomba
	if (level == MARIO_LEVEL_RACOON && !hasKilledGoomba && allowLanding % 2 == 0)
		SetState(MARIO_RACOON_STATE_LANDING);
	else if (!hasKilledGoomba)
		allowLanding = 0;

	//Giết xong thì quay lại sút cái mai
	if (hasKilledGoomba)
		SetState(MARIO_STATE_WALKING_RIGHT);

	if (isKicking && GetTickCount64() - kick_start > MARIO_KICK_TIME)
	{
		isKicking = false;
		kick_start = 0;
	}

	//Sút xong thì đi tới điểm gần phải đứng chờ
	if (hasKickKoopa && x > 170)
		SetState(MARIO_STATE_IDLE);

	HandleEvolving();
	UpdateSpeed();
	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"st, vy, ay: %d, %f, %f", state, vy, ay);
}

void CMarioNPC::HandleEvolving()
{
	if (GetTickCount64() - evolve_start >= MARIO_EVOLVE_TIME && isEvolving)
	{
		//Tiến hoá có 2 loại: tiến hoá lên và tiến hoá lùi
		if (isEvolveForward)
		{
			this->SetLevel(MARIO_LEVEL_RACOON);
			this->SetState(MARIO_RACOON_STATE_LANDING);
		}
		else //Tiến hoá ngược
			if (this->level == MARIO_LEVEL_RACOON)
				this->SetLevel(MARIO_LEVEL_SMALL); //Ở Intro thì từ Racoon về Small
		isEvolving = false;
		isEvolveForward = false;
		isEvolveBackward = false;
		evolve_start = 0;
	}
}

void CMarioNPC::UpdateSpeed()
{
	if (state == MARIO_STATE_WALKING_RIGHT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
		}
	}
}

void CMarioNPC::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
		OnCollisionWithBlockingObject(e);
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	//DebugOut(L"OCW\n");
}

void CMarioNPC::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMarioNPC::OnCollisionWithBlockingObject(LPCOLLISIONEVENT e)
{
	vy = 0;
	if (e->ny < 0)
		isOnPlatform = true;
}

void CMarioNPC::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		hasKilledGoomba = 1;
		e->obj->SetState(GOOMBA_STATE_DIE);
		vy = -0.4f; //nảy lên
		//Vẫn còn giật sau khi kill nó
	}
}

void CMarioNPC::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == KOOPA_STATE_SLIP)
		e->obj->SetState(KOOPA_STATE_SLEEP);
	else if (e->ny > 0)
	{
		//Bị dính chưởng từ trên đầu
		e->obj->SetSpeed(-0.07f, -0.08f);
		SetState(MARIO_STATE_GOT_HITTED_BY_SHELL);
	}
	else
	{
		this->SetState(MARIO_STATE_KICKING_RIGHT);
		hasKickKoopa = 1;
		e->obj->SetState(KOOPA_STATE_SLIP); //đạp nó
	}
}

void CMarioNPC::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CIntroPlayScene* intro_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	intro_scene->allowGoombaToMove = 1;

	this->isEvolveForward = true;
	this->SetState(MARIO_STATE_EVOLVING);
	e->obj->Delete();
	SpawnEffect(e, e->obj, EFF_COL_TYPE_SMOKE_EVOLVE);
}

void CMarioNPC::Render()
{
	//Big biến thành gấu mèo thì 0 vẽ trong 1 khoảng thgian
	if (isEvolving && level == MARIO_LEVEL_BIG && isEvolveForward) return; 

	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();
	animations->Get(aniId)->Render(x, y, false);
	//RenderBoundingBox();
}

int CMarioNPC::GetAniIdSmall()
{
	int aniId = -1;

	if (isEvolving && isEvolveForward)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
	}
	else
	{
		if (vx == 0)
		{
			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
			else if (ax == MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
			else if (ax == MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
			else if (ax == -MARIO_ACCEL_RUN_X)
				aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
			else if (ax == -MARIO_ACCEL_WALK_X)
				aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMarioNPC::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform) //ĐANG BAY || LƯỢN
	{
		if (isEvolving && isEvolveForward)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
		}
		else if (isEvolving && isEvolveBackward)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
		}
		else if (isHolding)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_JUMPING_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_HOLDING_JUMPING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
	{
		if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_KICKING_LEFT;
		}
		/*else if (isEvolving && isEvolveBackward) //tiến hoá lùi
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
		}*/
		else if (isHolding)
		{
			if (nx > 0 && vx > 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_WAKING_RIGHT;
			else if (nx < 0 && vx < 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_WAKING_LEFT;
			else if (nx > 0)
				aniId = ID_ANI_MARIO_BIG_HOLDING_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_HOLDING_LEFT;
		}
		else if (isSitting)
			aniId = ID_ANI_MARIO_SIT_LEFT;
		else if (isHitted)
			aniId = ID_ANI_MARIO_HITTED_BY_SHELL;
		else if (isLookUp)
			aniId = ID_ANI_MARIO_LOOK_UP;
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
		}
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMarioNPC::GetAniIdRacoon()
{
	int aniId = -1;

	if (!isOnPlatform) //included fly, jump, landing, fall
	{
		if (nx < 0 && isLanding)
		{
			aniId = ID_ANI_MARIO_RACOON_LANDING_LEFT;
		}
		else if (nx > 0 && isLanding)
		{
			aniId = ID_ANI_MARIO_RACOON_LANDING_RIGHT;
		}
		else if (nx > 0 && vy > 0) //Falling
		{
			aniId = ID_ANI_MARIO_RACOON_FALLING_RIGHT; //có vào đây
		}
		else if (nx < 0 && vy > 0)
		{
			aniId = ID_ANI_MARIO_RACOON_FALLING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT;
		}
	}
	else
	{
		if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_KICKING_LEFT;
		}
		else if (isHolding)
		{
			if (nx > 0 && vx > 0)
				aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_RIGHT;
			else if (nx < 0 && vx < 0)
				aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_LEFT;
			else if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_HOLD_LEFT;
		}
		else
		{
			if (vx == 0)
				if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACOON_IDLE_LEFT;
			else if (vx > 0)
				aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
			else // vx < 0
				if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_LEFT;
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;

	return aniId;
}

void CMarioNPC::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		maxVx = 0.2f;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		maxVx = -0.0f;
		vx = 0;
		//ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		maxVx = 0.07f;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;

		break;

	case MARIO_STATE_WALKING_LEFT:
		maxVx = -0.075f;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;

		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			isSitting = true;
			vx = 0.0f; 
			vy = 0.0f;
			ax = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			//ay = 0.00042f;
			isSitting = false;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_KICKING_RIGHT:
		isKicking = true;
		kick_start = GetTickCount64(); //Tính từ khi chạy ctrinh tới bây giờ

		break;

	case MARIO_STATE_JUMPING:
		if (isOnPlatform)
		{
			isJumping = true;
			isLanding = false;
			isLookUp = false;
			look_timeout = 0;
			vy = -0.78f;
		}
		break;

	case MARIO_RACOON_STATE_LANDING:
	{
		isLanding = true;
		isJumping = false;
		allowLanding = 1;
		ay = 0.005f;

		vx = -0.055f;
		nx = -1;
		vy = 0.0f; //Set lại vy = 0 nếu đang Landing
		DebugOut(L"Landing\n");
		break;
	}

	/*case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0)
			vy += 0.37f / 2.0f; //Nhảy cao hơn
		isJumping = false;
		break;*/ //Consider this state

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;

		break;

	case MARIO_STATE_EVOLVING:
		evolve_start = GetTickCount64();
		isEvolving = true;

		break;

	case MARIO_STATE_HOLDING:
		//nothing special
		break;

	case MARIO_STATE_GOT_HITTED_BY_SHELL:
		isHitted = true;
		hitted_timeout = GetTickCount64();
		break;

	case MARIO_STATE_LOOKUP:
		isHitted = false;
		isLookUp = true;
		look_timeout = GetTickCount64();

		break;
	}

	CGameObject::SetState(state);
}

void CMarioNPC::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Really IMPORTANT
	if (level != MARIO_LEVEL_SMALL) //Big or Racoon
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMarioNPC::SpawnEffect(LPCOLLISIONEVENT e, LPGAMEOBJECT obj, int eff_type)
{
	float x = -1;
	float y = -1;

	//tiến hoá, cây chết 
	{
		x = this->x;
		y = this->y;
	}

	CEffectCollision* eff_col = new CEffectCollision(x, y, GetTickCount64(), eff_type);
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff_col, 0);
}

void CMarioNPC::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	level = l;
}