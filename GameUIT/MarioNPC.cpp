#include "MarioNPC.h"
#include "ColorPlatform.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mario.h"

void CMarioNPC::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMarioNPC::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CColorPlatform*>(e->obj))
		OnCollisionWithColorPlatform(e);
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
}

void CMarioNPC::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMarioNPC::OnCollisionWithColorPlatform(LPCOLLISIONEVENT e)
{
	CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
	HandleCollisionWithColorPlatform(e, cl_pf);
}

void CMarioNPC::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	//Hàm này để xử lý vị trí của Mario khi nhảy lên Color Platform
	//Nhằm tránh việc nó bị rơi xuống dù có va chạm
	//Tương tự như hàm SetLevel trong framework của thầy.

	//Condition that I learned: |Player.TopY + Player.Height > ColorPlat.TopY => SNAP|
	if (this->level != MARIO_LEVEL_SMALL) //level gấu mèo và BIG dùng chung BBox được vì diff 0 đáng kể
	{

		if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
		{
			//SNAP: Player.Y = ColorPlat.Y - ColorPlat.Height / 2 - Player.Height / 2
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
			vy = 0;
		}
	}
	else
	{
		if (this->y + MARIO_SMALL_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
		{
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - MARIO_SMALL_BBOX_HEIGHT / 2;
			vy = 0;
		}
	}
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
		maxVx = -0.2f;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;

		break;

	case MARIO_STATE_WALKING_LEFT:
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;

	case MARIO_STATE_SIT:	//set state ngồi ở Keyboard

		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			isSitting = true;
			vx = 0; vy = 0.0f;
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
	}

	CGameObject::SetState(state);
}