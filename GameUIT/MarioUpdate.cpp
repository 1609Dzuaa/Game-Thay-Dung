#include "Mario.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Phải dừng vận tốc ở đây chứ 0 phải ở hàm OnNoCollision
	//Vì ở đây gọi trc r mới đến OnNoCollision
	//Cập nhật vị trí Koopa khi đc ôm bởi Mario

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
	if (isTravelDown)
		HandleTravellingDown();
	else if (isTravelUp)
		HandleTravellingUp();

	if (this->y >= END_POS)
	{
		isReachEndPos = true;
		return; //ngưng update
	}

	if (coin == 100)
	{

	}

	UpdateMarioState();
	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (isAttacking && !isInitialized)
	{
		tail = new CTail(x, y, MARIO_BIG_BBOX_WIDTH, nx, vx);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		current_scene->AddObjectToScene(tail);
		isInitialized = true;
		DebugOut(L"Tail was created successfully!\n");
	}

	//DebugOutTitle(L"x, y, isWait, atMW: %f, %f, %d, %d", x, y, isWaitingForTrans, isAtMainWorld);
	//DebugOutTitle(L"St, vx, End: %d, %f, %d", state, vx, isEndGame);
}

void CMario::UpdateMarioState()
{
	// reset untouchable timer if untouchable time has passed

	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount64() - shaking_start > SHAKING_TIME)
	{
		shaking_start = 0;
		Shaking = false;
	}

	if (GetTickCount64() - fly_start > FLYING_TIME && isFlying)
	{
		fly_start = 0;
		vy = 0;
		isFlying = false;
		canFly = false;
	}

	if (GetTickCount64() - evolve_start >= MARIO_EVOLVE_TIME && isEvolving)
	{
		//Tiến hoá có 2 loại: tiến hoá lên và tiến hoá lùi
		if (isEvolveForward)
		{
			if (this->level == MARIO_LEVEL_SMALL)
				this->SetLevel(MARIO_LEVEL_BIG);
			else
				this->SetLevel(MARIO_LEVEL_RACOON);
		}
		else //Tiến hoá ngược
		{
			if (this->level == MARIO_LEVEL_RACOON)
				this->SetLevel(MARIO_LEVEL_BIG);
			else if (this->level == MARIO_LEVEL_BIG)
				this->SetLevel(MARIO_LEVEL_SMALL);
		}
		isEvolving = false;
		isEvolveForward = false;
		isEvolveBackward = false;
		isAteItem = false;
		evolve_start = 0;
	}

	if (this->state == MARIO_STATE_RUNNING_RIGHT || this->state == MARIO_STATE_WALKING_RIGHT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_RIGHT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_SPEED_RIGHT);
		}
	}
	else if (this->state == MARIO_STATE_RUNNING_LEFT || this->state == MARIO_STATE_WALKING_LEFT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_LEFT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_SPEED_LEFT);
		}
	}
	else if (this->isAtMaxSpeed && isJumping && isOnPlatform)
		this->SetState(MARIO_STATE_JUMP_AT_MAX_SPEED);

	if (isKicking && GetTickCount64() - kick_start >= MARIO_KICK_TIME)
	{
		isKicking = false;
		kick_start = 0;
	}

	if (isAttacking && GetTickCount64() - attack_start >= MARIO_RACOON_ATTACK_TIME)
	{
		attack_start = 0;
		isAttacking = false;
		isInitialized = false;
		tail->Delete();
		DebugOut(L"Delete Tail Success\n");
	}
}