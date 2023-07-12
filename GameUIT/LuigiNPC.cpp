#include "LuigiNPC.h"
#include "Mario.h"
#include "SMB3Curtain.h"
#include "IntroPlayScene.h"
#include "MarioNPC.h"

void CLuigiNPC::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	if (x > 71.0f && !hasJumped)
	{
		SetState(MARIO_STATE_JUMPING);
		hasJumped = 1;
	}

	if (y < 50 && !hasSpawn)
	{
		SpawnSuperMarioBros3(); //Kéo rèm SMB3
		hasSpawn = 1;
	}

	if (y > 300)
	{
		ghost_mario_npc->SetState(MARIO_STATE_SIT_RELEASE);
		ghost_mario_npc = nullptr;
		this->Delete();
	}

	UpdateSpeed();
	isOnPlatform = false;

	//DebugOutTitle(L"vx, maxVx, st: %f, %f, %d", vx, maxVx, state);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLuigiNPC::UpdateSpeed()
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

void CLuigiNPC::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLuigiNPC::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking() && e->ny < 0)
	{
		vy = 0;
		isOnPlatform = true;
	}
	else if (dynamic_cast<CMarioNPC*>(e->obj))
		OnCollisionWithMarioNPC(e);
}

void CLuigiNPC::OnCollisionWithMarioNPC(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		vy = -0.8f;
		e->obj->SetState(MARIO_STATE_SIT);
		ghost_mario_npc = dynamic_cast<CMarioNPC*>(e->obj);
	}
}

void CLuigiNPC::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetAniID())->Render(x, y, false);
}

int CLuigiNPC::GetAniID()
{
	int aniId = -1;
	if (!isOnPlatform) //ĐANG BAY || LƯỢN
	{
		if (nx >= 0)
			aniId = ID_ANI_LUIGI_JUMPING_RIGHT;
		else
			aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
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
				if (nx > 0) aniId = ID_ANI_LUIGI_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_LUIGI_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = -1;
			}
		}
	}
	if (aniId == -1) aniId = ID_ANI_LUIGI_IDLE_RIGHT;

	return aniId;
}

void CLuigiNPC::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		maxVx = 0.2f;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		maxVx = -0.00224f;
		vx = -0.001f;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		maxVx = 0.07f;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;

		break;

	case MARIO_STATE_WALKING_LEFT:
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;

	case MARIO_STATE_KICKING_LEFT:
		isKicking = true;
		kick_start = GetTickCount64(); //Tính từ khi chạy ctrinh tới bây giờ
		break;

	case MARIO_STATE_JUMPING:
		if (isOnPlatform)
		{
			isJumping = true;
			isOnPlatform = false;
			vy = -0.6f;
		}
		break;

	/*case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0)
			vy += 0.37f / 2.0f; //Nhảy cao hơn
		isJumping = false;
		break;*/ //Consider this state

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;

		break;

	case MARIO_STATE_HOLDING:
		//nothing special
		break;
	}

	CGameObject::SetState(state);
}

void CLuigiNPC::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Really IMPORTANT
	left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
	top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
	right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
	bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
}

void CLuigiNPC::SpawnSuperMarioBros3()
{
	CSMB3Curtain* smb3_curtain = new CSMB3Curtain(127, -120);
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddSubObjectToScene(smb3_curtain);
}