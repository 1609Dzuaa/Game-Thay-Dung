#include "Mario.h"
#include "Camera.h"

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = 0.2f;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		isRunning = true;
		break;

	case MARIO_STATE_RUNNING_AT_MAX_SPEED_RIGHT: //Ở state này thì mới cho bay
		if (isSitting) break;
		isAtMaxSpeed = true;
		if (this->level == MARIO_LEVEL_RACOON && SpeedBar == 7)
			canFly = true;
		isRunning = true;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -0.2f;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;

	case MARIO_STATE_RUNNING_AT_MAX_SPEED_LEFT:
		if (isSitting) break;
		isAtMaxSpeed = true;
		if (this->level == MARIO_LEVEL_RACOON && SpeedBar == 7)
			canFly = true;
		isRunning = true;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		if (!isEndGame)
		{
			maxVx = MARIO_WALKING_SPEED;
			ax = MARIO_ACCEL_WALK_X;
		}
		else
			vx = MARIO_WALKING_SPEED;

		nx = 1;
		isWalking = true;
		break;

	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		isWalking = true;
		break;

	case MARIO_STATE_KICKING_RIGHT:
		if (isSitting) break;
		isKicking = true;
		kick_start = GetTickCount64(); //Tính từ khi chạy ctrinh tới bây giờ
		break;

	case MARIO_STATE_KICKING_LEFT:
		if (isSitting) break;
		isKicking = true;
		kick_start = GetTickCount64();
		break;

	case MARIO_STATE_JUMPING:
		if (isSitting) break;
		if (isOnPlatform)
		{
			isJumping = true;
			isLanding = false;
			if (isRunning)
				vy = -0.57f;
			else
				vy = -0.52f;
		}
		break;

	case MARIO_STATE_JUMP_AT_MAX_SPEED:
		if (isOnPlatform)
		{
			isJumping = true;
			vy = -0.6f;
		}
		break;

	case MARIO_RACOON_STATE_LANDING:
	{
		isLanding = true;
		isJumping = false;
		isFlying = false;
	    
		vy = 0.0f; //Set lại vy = 0 nếu đang Landing
		DebugOut(L"Landing\n");
		break;
	}

	case MARIO_RACOON_STATE_FLYING:
	{
		if (!isFlying)
		{
			isFlying = true;
			isAtMaxSpeed = true;
			isRunning = false;
			isLanding = false;
			isJumping = false;
			fly_start = GetTickCount64();
			vy = -MARIO_FLYING_SPEED;
		}
		else 
			vy = -MARIO_FLYING_SPEED; //Nếu đang bay thì bơm thêm Vy cho Mario
		break;
	}

	case MARIO_STATE_RELEASE_JUMP: 
		if (vy < 0) 
			vy += 0.37f / 2.0f; //Nhảy cao hơn
		isJumping = false; 
		break;

	case MARIO_STATE_SIT:	//set state ngồi ở Keyboard
		if (isAtMaxSpeed) return;	//0 cho ngồi
		if (isRunning) return;
		if (isWalking)
			return;

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
			ay = 0.00042f;
			isSitting = false;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_RACOON_STATE_ATTACK:
	{
		if (isAttacking) break;
		
		attack_start = GetTickCount64();
		isAttacking = true;
		isKicking = false;
		isSitting = false;
	}
	break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		if (!isEndGame)
		{
			vx = 0.0f;
			isWalking = false;
			isRunning = false;
		}
		else
			state = MARIO_STATE_WALKING_RIGHT;

		break;

	case MARIO_STATE_EVOLVING:
		evolve_start = GetTickCount64();
		isEvolving = true;
		break;

	case MARIO_STATE_HOLDING:
		//nothing special
		break;

	case MARIO_STATE_TRAVELLING:
		isTravelling = true;
		ay = 0;

		if (isTravelDown)
			vy = MARIO_TRAVELLING_SPEED;
		else if (isTravelUp)
			vy = -MARIO_TRAVELLING_SPEED;

		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED * 1.5f;
		vx = 0;
		ax = 0;
		HP--;
		CCamera::GetInstance()->SetTargetToFollow(nullptr); //Neccessary?
		break;
	}

	CGameObject::SetState(state);
}