#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"

CMario::CMario(float x, float y) : CGameObject(x, y)
{
	isIdleing = false;
	isSitting = false;
	isWalking = false;
	isRunning = false;
	isKicking = false;
	isAttacking = false;
	isJumping = false;
	isFlying = false;
	canFly = false;
	isAtMaxSpeed = false;
	isFalling = false;
	isLanding = false;
	isEvolving = false;
	isEvolveForward = false;
	isEvolveBackward = false;
	isAteItem = false;
	StopWatch = false;
	Shaking = false;
	isAllowToHoldKoopa = false;
	isHolding = false;
	isHitSwitch = false;
	isInitialized = false;
	CountJumpOnEnemies = 0;
	untouchdraw = -1;
	untouch_draw_0 = 0;
	untouch_draw_1 = 0;
	untouch_0 = 0;
	untouch_1 = 0;
	shaking_start = 0;
	shakeUp_start = 0;
	shakeDown_start = 0;
	isShakeUp = 0;
	tail = NULL;
	//Thêm đuôi trước tương tự như mushroom
	ghost_koopa = NULL; //Khi đang Hold Koopa thì coi nó như thuộc tính của Mario
	//Giống như bật khiên

	maxVx = 0;
	maxRunningSpeed = MARIO_RUNNING_SPEED;
	ax = 0.0f;
	ay = MARIO_GRAVITY;

	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	untouchable_start = -1;
	kick_start = 0;
	attack_start = 0;
	evolve_start = 0;
	isOnPlatform = false;
	coin = 0;
}

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
	DebugOutTitle(L"Vy: %f", vy);
}

void CMario::UpdateMarioState()
{
	// reset untouchable timer if untouchable time has passed
	//when untouchable, there are 2 states of drawing: Draw & NOT draw

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

	if (this->level == MARIO_LEVEL_RACOON && vy > 0 && !isOnPlatform)
	{
		//Nếu là Racoon và vector vy hướng xuống cũng như không ở trên nền
		//ưu tiên Landing trước Falling
	//	if (state == MARIO_RACOON_STATE_LANDING); //prob here
		//else
		//this->SetState(MARIO_RACOON_STATE_FALLING);
		//Khi đang bay ở trạng thái Racoon và vy đổi dấu
		//thì bật hoạt ảnh falling lên
	}

	if (this->state == MARIO_STATE_RUNNING || this->state == MARIO_STATE_WALKING)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx; //Giới hạn vận tốc cho nó
			if (this->state == MARIO_STATE_RUNNING)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_VX);
		}
	}
	else if (this->isAtMaxSpeed && isJumping)
		this->SetState(MARIO_STATE_JUMPING_AT_MAX_SPEED);

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

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (!isAteItem) //Nếu đc SetLevel bằng cách nhấn phím thì giảm y để Mario kh bị rơi xuống nền
		if (this->level == MARIO_LEVEL_SMALL)
		{
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2; //Key problem here
		}

	level = l;
}

void CMario::SpawnScore(LPGAMEOBJECT obj)
{
	CEffectScore* eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, NORMAL_SCORE);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(ClassifyScore(obj, eff_scr));
}

CEffectScore* CMario::ClassifyScore(LPGAMEOBJECT obj, CEffectScore* eff_scr)
{
	if (!obj->IsSpecialItem())
	{
		switch (CountJumpOnEnemies)
		{
		case 1:
			eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, NORMAL_SCORE);
			break;
		case 2:
			eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, DOUBLE_SCORE);
			break;
		case 3:
			eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, QUADRA_SCORE);
			break;
		case 4:
			eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, DOUBLE_QUADRA_SCORE);
			break;
		}
	}
	else
	{
		if (dynamic_cast<CMushroom*>(obj))
		{
			CMushroom* mr = dynamic_cast<CMushroom*>(obj);
			if(mr->GetType() == RED_MUSHROOM)
				eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, ITEM_SCORE);
			else
				eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, ITEM_SCORE_LV_UP);
		}
		else
			eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, ITEM_SCORE);
	}
	return eff_scr;
	//Hàm sàng lọc điểm
}

void CMario::SpawnEffect(LPCOLLISIONEVENT e, LPGAMEOBJECT obj, int eff_type)
{
	float x = -1;
	float y = -1;
	if (eff_type != EFF_COL_TYPE_SMOKE_EVOLVE)
	{
		if (e->nx > 0)
			x = obj->GetX() - 15.0f;
		else
			x = obj->GetX() + 15.0f;
		y = obj->GetY();
	}
	else //tiến hoá, cây chết 
	{
		x = this->x;
		y = this->y;
	}

	CEffectCollision* eff_col = new CEffectCollision(x, y, GetTickCount64(), eff_type);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff_col);
}

int CMario::KoopaStateThatAllowToHold(CKoopa* koopa)
{
	return (koopa->GetState() == KOOPA_STATE_SLEEP || koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE
		|| koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL || koopa->GetState() == KOOPA_STATE_REBORN
		|| koopa->GetState() == KOOPA_STATE_REBORN_REVERSE);
}