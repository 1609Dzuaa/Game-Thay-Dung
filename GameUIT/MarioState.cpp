#include "Mario.h"

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING:
		isRunning = true;
		if (nx > 0)
		{
			maxVx = MARIO_RUNNING_SPEED;
			ax = MARIO_ACCEL_RUN_X;
		}
		else if (nx < 0)
		{
			maxVx = -MARIO_RUNNING_SPEED;
			ax = -MARIO_ACCEL_RUN_X;
		}
		break;

	case MARIO_STATE_RUNNING_AT_MAX_VX:
		isAtMaxSpeed = true;
		if (this->level == MARIO_LEVEL_RACOON)
			canFly = true;
		break;

	case MARIO_STATE_WALKING:
		isWalking = true;
		isIdleing = false;
		if (isSitting)
		{
			isSitting = false;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		if (nx > 0)
		{
			maxVx = MARIO_WALKING_SPEED;
			ax = MARIO_ACCEL_WALK_X;
		}
		else if (nx < 0)
		{
			maxVx = -MARIO_WALKING_SPEED;
			ax = -MARIO_ACCEL_WALK_X;
		}
		break;

	case MARIO_STATE_KICKING:
		if (isSitting) break; //does it neccessary ?!
		isKicking = true;
		kick_start = GetTickCount64();
		break;

	case MARIO_STATE_JUMPING:
		if (isSitting) break;
		if (isOnPlatform)
		{
			isJumping = true;
			isLanding = false;
			isFalling = false;
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_JUMPING_AT_MAX_SPEED:
		if (isSitting) break;
		if (isOnPlatform)
		{
			isJumping = true;
			isLanding = false;
			isFalling = false;
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
			break;
		}

	case MARIO_RACOON_STATE_FALLING:
	{
		isFalling = true;
		isJumping = false;
		isLanding = false;
		break;
	}

	case MARIO_RACOON_STATE_LANDING:
	{
		isLanding = true;
		isFalling = false;
		isJumping = false;
		isFlying = false;
	    
		vy = 0.0f; //Set lại vy = 0 nếu đang Landing
		DebugOut(L"Landing\n");
		break;
	}

	case MARIO_RACOON_STATE_FLYING:
	{
		isFlying = true;
		isLanding = false;
		isFalling = false;
		isJumping = false;
		vy = -0.2f;

		break;
	}

	case MARIO_STATE_RELEASE_JUMP: //prob here
		if (vy < 0) 
			vy += MARIO_JUMP_SPEED_Y / 2;
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
			isSitting = false;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_RACOON_STATE_ATTACK:
	{
		if (isAttacking) break;
		if (isIdleing)
			isIdleing = false;

		attack_start = GetTickCount64();
		isAttacking = true;
		//isIdleing = false;
		isKicking = false;
		isSitting = false;
	}
	break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		isIdleing = true;
		isWalking = false;
		isRunning = false;
		isFlying = false;
		isLanding = false;
		isKicking = false;
		isAttacking = false;
		isJumping = false;
		isAtMaxSpeed = false;

		break;

	case MARIO_STATE_EVOLVING:
		evolve_start = GetTickCount64();
		isEvolving = true;
		break;

	case MARIO_STATE_HOLDING:
		//nothing special
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED * 1.5f;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}