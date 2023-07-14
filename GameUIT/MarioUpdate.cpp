#include "Mario.h"
#include "PlayScene.h"
#include "BlackScreen.h"
#include "DataBinding.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Phải dừng vận tốc ở đây chứ 0 phải ở hàm OnNoCollision
	//Vì ở đây gọi trc r mới đến OnNoCollision
	//Cập nhật vị trí Koopa khi đc ôm bởi Mario
	//Tạo 1 biến đếm độ max speed để cho phép chạy
	//Chỉ khi trên mặt đất thì biến đấy mới tăng dần lên
	//Còn nếu đang chạy mà nhảy (not on platform) thì cái biến đấy giảm dần

	//chỉnh Grav ở đây :vvv, mò mãiii
	//Quên mất phải != state Travel nếu 0 sẽ bị tụt ngay lập tức khi travel
	if (isOnPlatform && state != MARIO_STATE_DIE && state != MARIO_STATE_TRAVELLING)
		ay = MARIO_GRAVITY_ON_PLATFORM; //để 0 bị trong trạng thái isOnPlat && !isOnPlat cùng 1 lúc
	else if(!isOnPlatform && state != MARIO_STATE_DIE && state != MARIO_STATE_TRAVELLING)
		ay = MARIO_GRAVITY_NOT_ON_PLATFORM; //cảm giác sẽ bay bổng hơn

    //DEAD_ZONE
	if (isAtMainWorld && y > 350)
		SetState(MARIO_STATE_DIE);

	if (GetTickCount64() - die_idle_start > 900 && isDieIdle) //Make sure only 1 time
	{
		isDieIdle = false;
		isDieJump = true;
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		ay = MARIO_GRAVITY_NOT_ON_PLATFORM;
		die_idle_start = 0;
		die_time_out = GetTickCount64();
	}
	//If: Lấy Timer ở PlayScene == 0 và ReachEndPos => bấm giờ để tắt màn cũng như chuyển scene
	//Sau đó 
	CPlayScene* play_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (state == MARIO_STATE_DIE && !isDieIdle || isReachEndPos && play_scene->GetTimer() == 0)
	{
		if (GetTickCount64() - die_time_out > 1500 && isDieJump && !init)
		{
			//tắt Cam chứ 0 thì nó bị lệch xuống dưới
			//Hết chết nhảy -> chuyển scene về WORLD
			isDieJump = false;
			//DebugOut(L"CamX, CamY: %f, %f\n", CCamera::GetInstance()->GetCamPos().x, CCamera::GetInstance()->GetCamPos().y);
			CBlackScreen::GetInstance()->SetState(BLACK_SCR_EFF_STATE_DRAW_FROM_0);
			init = 1;
		}
		else if (state != MARIO_STATE_DIE && !initWaitEnd)
		{
			//prob here
			initWaitEnd = true;
			isWaitEndGame = true;
			wait_end_game = GetTickCount64();
		}

		if (isWaitEndGame && GetTickCount64() - wait_end_game >= 1500)
		{
			isWaitEndGame = false;
			CBlackScreen::GetInstance()->SetState(BLACK_SCR_EFF_STATE_DRAW_FROM_0);
		}

		if (CBlackScreen::GetInstance()->GetAlpha() == 1.0f)
		{
			isAtWorld = 1;
			isAtMainWorld = 0;
			isTrulyDied = true;
			isTrulyEnd = true;
			CGame::GetInstance()->InitiateSwitchScene(ID_WORLD);

			if (isEndGame)
			{
				CDataBindings::GetInstance()->WorldEntrance[CDataBindings::NumEntrancePass].isPassed = 1;
				CDataBindings::NumEntrancePass++;
				CDataBindings::Mario_Level = this->level;
				//End game rồi thì mới tính là pass (Obiviously!)
			}
			return;
		}
	}
	//DebugOut(L"CamX, CamY: %f, %f\n", CCamera::GetInstance()->GetCamPos().x, CCamera::GetInstance()->GetCamPos().y);
	//DebugOutTitle(L"St, atMW, atW: %d, %d, %d", state, isAtMainWorld, isAtWorld);
	//DebugOutTitle(L"waitEnd, InitWait: %d, %d", isWaitEndGame, initWaitEnd);
	//DebugOutTitle(L"CardType: %d", TypeOfCardCollected);
	
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

	if (this->x >= 3000.0f && !isWaitEndGame)
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
	HandleUntouchableDrawState(); //đặt đây thì hợp lý hơn
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

	if (ghost_koopa)
		DebugOutTitle(L"x, y, st: %f, %f, %d", ghost_koopa->GetX(), ghost_koopa->GetY(), ghost_koopa->GetState());
	//DebugOutTitle(L"SpeedBar, prevVx, vx, MS: %d, %f, %f, %d", SpeedBar, prevVx, vx, isAtMaxSpeed);
	//DebugOutTitle(L"vx, ax, ay, st: %f, %f, %f, %d", vx, ax, ay, state);
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
			{
				isAtMaxSpeed = true;
				if (this->level == MARIO_LEVEL_RACOON && SpeedBar == 7)
					canFly = true;
			}
				//this->SetState(MARIO_STATE_RUNNING_AT_MAX_SPEED_RIGHT); consider this state
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

void CMario::HandleUntouchableDrawState()
{
	//when untouchable, there are 2 states of drawing: Draw & NOT draw

	//0 vẽ
	if (untouchable && GetTickCount64() - untouch_draw_0 >= UNTOUCH_DRAW_TIME && untouch_0)
	{
		untouch_0 = 0;
		untouch_1 = 1;
		untouch_draw_0 = 0;
		untouch_draw_1 = GetTickCount64();
	} //vẽ
	else if (untouchable && GetTickCount64() - untouch_draw_1 >= UNTOUCH_DRAW_TIME && untouch_1)
	{
		untouch_0 = 1;
		untouch_1 = 0;
		untouch_draw_0 = GetTickCount64();
		untouch_draw_1 = 0;
	}
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
		CDataBindings::GetInstance()->IsStopWatch = 0;
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
	//Tích thanh nộ THEO THỜI GIAN chứ KHÔNG PHẢI vận tốc :vvv

	// Đang chạy tích lực:
	// 0.| Start_Time(luôn đc cập nhật) -(900ms)-> 1.| -(115ms)-> 2.| ... --> MaxSpeedBar
	// giữ A và chạy hết 900 => tăng 1 nấc, từ đó trở đi mỗi 115ms tăng 1 nấc
	if (isRunning && isOnPlatform) //Chỉ đang chạy mới tăng đc SpeedBar
	{
		if (GetTickCount64() - run_start > START_RUN_TIME_OUT)
		{
			//Vào đc đây là cho phép tăng SpeedBar
			if (GetTickCount64() - speed_bar_start > 115)
			{
				if (SpeedBar < 7)
				{
					SpeedBar++;
					//maxVx += SpeedBar * 0.001f; Necessary ?
				}
				speed_bar_start = GetTickCount64();	//Cập nhật lại thời gian tại thời điểm vừa đc tăng
			}
		}
	}
	else if (ax * vx < 0 || !isRunning || isRunning && !isOnPlatform)
	{
		if (isFlying) return;
		//Giảm dần SpeedBar mỗi 115ms nếu 0 Chạy
		if (GetTickCount64() - speed_bar_stop > 115)
		{
			if (SpeedBar > 0)
				SpeedBar--;
			speed_bar_stop = GetTickCount64(); //Cập nhật lại thời gian tại thời điểm vừa đc giảm
		}
		run_start = GetTickCount64(); //Thằng này sẽ luôn đc cập nhật để biết mốc thgian tính từ lúc có chạy tích lực
	}
		//Các TH SpeedBar bị giảm trong game:
		//Không chạy tích lực nữa VÀ đang ở trên nền (Done!)
		//Đang chạy NHƯNG va phải vật
		//Đang Bay và hết thời gian bay (Done!)
		//Đang chạy NHƯNG phanh lại
		//Đang chạy NHƯNG lại nhảy khi chưa maxspeed (Done!)
	//DebugOutTitle(L"RT, start, st: %d, %d, %d", run_start, speed_bar_start, speed_bar_stop);
}