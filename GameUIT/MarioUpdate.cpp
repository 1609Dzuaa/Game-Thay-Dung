#include "Mario.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Phải dừng vận tốc ở đây chứ 0 phải ở hàm OnNoCollision
	//Vì ở đây gọi trc r mới đến OnNoCollision
	//Cập nhật vị trí Koopa khi đc ôm bởi Mario
	//Tạo 1 biến đếm độ max speed để cho phép chạy
	//Chỉ khi trên mặt đất thì biến đấy mới tăng dần lên
	//Còn nếu đang chạy mà nhảy (not on platform) thì cái biến đấy giảm dần

	if (state == MARIO_STATE_DIE || isReachEndPos)
	{
		//tắt Cam chứ 0 thì nó bị lệch xuống dưới
		isAtMainWorld = false;
		DebugOut(L"CamX, CamY: %f, %f\n", CCamera::GetInstance()->GetCamPos().x, CCamera::GetInstance()->GetCamPos().y);
		CGame::GetInstance()->InitiateSwitchScene(ID_WORLD);
		return;
	}
	DebugOut(L"CamX, CamY: %f, %f\n", CCamera::GetInstance()->GetCamPos().x, CCamera::GetInstance()->GetCamPos().y);
	DebugOutTitle(L"St, vy, ay: %d, %f, %f", state, vy, ay);

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

	if (this->x >= 3000.0f)
	{
		isReachEndPos = true;
		wait_end_game = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		return; //ngưng update
	}

	UpdateTime();
	UpdateSpeedBar();
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

	//DebugOutTitle(L"vx, ax, maxVx ST: %f, %f, %f, %d", vx, ax, maxVx, state);
	//DebugOutTitle(L"SpeedBar, prevVx, vx, MS: %d, %f, %f, %d", SpeedBar, prevVx, vx, isAtMaxSpeed);
	//DebugOut(L"%d\n", SpeedBar);
}

void CMario::UpdateMarioState()
{
	//Walk & Run
	if (state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_WALKING_RIGHT
		|| state == MARIO_RACOON_STATE_FLYING)
	{
		//Thêm ĐK này: vy >= 0 && vx > 0 || vy >= 0 && vx <0
		//vì có thể khi 0 bay nữa mà giữ nút A thì đâm ra vận tốc rất lớn
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_RIGHT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_SPEED_RIGHT);
		}
	}
	if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_WALKING_LEFT
		|| state == MARIO_RACOON_STATE_FLYING)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_LEFT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_SPEED_LEFT);
		}
	}
	if (this->isAtMaxSpeed && isJumping && isOnPlatform)
		this->SetState(MARIO_STATE_JUMP_AT_MAX_SPEED);
}

void CMario::UpdateTime()
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
		isAtMaxSpeed = false; 
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

void CMario::UpdateSpeedBar()
{
	//if (SpeedBar == 0) prevVx = 0; //Reset prevVx
	//Vẫn còn trường hợp đang chạy max nhưng speedbar 0 đạt tới max
	//xem xét giảm vx dựa trên speedbar ?
	//Nếu di chuyển bthg rồi 1 lúc sau tăng tốc (A) thì SpeedBar 0 đạt max

	//Chia maxSpeed thành 7 khúc ~ nhau
	if (isRunning && SpeedBar < 7 && isOnPlatform)	//Chỉ Running mới tích đc lực
	{
		//7 phần như nhau => cắt 8 khúc
		if (vx > 0)
		{
			if (this->vx - prevVx >= maxVx / 8)
			{
				SpeedBar++;
				prevVx = vx;
			}
		}
		else if (vx < 0)
		{
			if (this->vx - prevVx <= maxVx / 8)
			{
				SpeedBar++;
				prevVx = vx;
			}
		}
	}
	else if (SpeedBar > 0) //SpeedBar phải > 0 thì mới giảm đc (obviously)
	{
		//Các TH SpeedBar bị giảm trong game:
		//Không chạy tích lực nữa VÀ đang ở trên nền (Done!)
		//Đang chạy NHƯNG va phải vật
		//Đang Bay và hết thời gian bay (Done!)
		//Đang chạy NHƯNG phanh lại
		//Đang chạy NHƯNG lại nhảy khi chưa maxspeed (Done!)
		if (!isRunning && isOnPlatform || !isAtMaxSpeed || ax * vx < 0)
		{
			//Để khi bay nhưng 0 đạt MaxSpeed thì nó vẫn 0 giảm SB
			if (isFlying) return;
			//đôi lúc khi bay sẽ thấy bay theo trục x nhan vcl @@, cần giới hạn nó
			//Bug brace khi chạy, nó sẽ 0 brace mà chạy lại hướng ngược lại
			if (abs(vx - prevVx) >= abs(maxVx / 8))
			{
				SpeedBar--;
				if (SpeedBar == 0) 
					prevVx = 0; //Reset prevVx
				else 
					prevVx -= abs(maxVx / 8);
			}
		}
	}
}