#include "MarioNPC.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "debug.h"

void CMarioNPC::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	//if (state == MARIO_STATE_SIT) vx = 0;
	UpdateSpeed();
	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"st: %d", state);
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
	DebugOut(L"OCW\n");
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
	e->obj->SetState(GOOMBA_STATE_DIE);
	vy = -0.274f; //nảy lên
}

void CMarioNPC::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == KOOPA_STATE_SLIP)
		e->obj->SetState(KOOPA_STATE_SLEEP);
	else
		e->obj->SetState(KOOPA_STATE_SLIP);
}

void CMarioNPC::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	//else if (level == MARIO_LEVEL_SMALL)
	//	aniId = GetAniIdSmall();
	//else if (level == MARIO_LEVEL_RACOON)
	//	aniId = GetAniIdRacoon();
	animations->Get(aniId)->Render(x, y, false);
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
		maxVx = 0.0023f;
		vx = 0.001f;
		ax = MARIO_ACCEL_RUN_X;
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
			vy = -0.55f;
		}
		break;

	case MARIO_RACOON_STATE_LANDING:
	{
		isLanding = true;
		isJumping = false;

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
		//CDataBindings::GetInstance()->IsStopWatch = 1;

		break;

	case MARIO_STATE_HOLDING:
		//nothing special
		break;

	case MARIO_STATE_GOT_HITTED_BY_SHELL:
		isHitted = true;
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