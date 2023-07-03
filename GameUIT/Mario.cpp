#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "Tube.h"

CMario::CMario(float x, float y) : CGameObject(x, y)
{
	//isIdleing = false;
	isSitting = false;
	isWalking = false;
	isRunning = false;
	isKicking = false;
	isAttacking = false;
	isJumping = false;
	isFlying = false;
	canFly = false;
	isAtMaxSpeed = false;
	isLanding = false;
	isEvolving = false;
	isEvolveForward = false;
	isEvolveBackward = false;
	isAteItem = false;
	StopWatch = false;
	Shaking = false;
	isAllowToHoldKoopa = false;
	isAllowToUseTube = false;
	isTravelling = false;
	isHolding = false;
	isHitSwitch = false;
	isInitialized = false;
	isTravelUp = false;
	isTravelDown = false;
	isAtMainWorld = true;
	isComboUpAndS = false;
	CountJumpOnEnemies = 0;
	untouchdraw = -1;
	untouch_draw_0 = 0;
	untouch_draw_1 = 0;
	untouch_0 = 0;
	untouch_1 = 0;
	shaking_start = 0;
	start_y = -1;
	fly_start = -1;
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

void CMario::HandleTravellingDown()
{
	if (isTravelling && y - start_y > MARIO_BIG_BBOX_HEIGHT && isAtMainWorld)
	{
		isAtMainWorld = false;
		SetPosition(DOWN_TUBE_POSITION_X, DOWN_TUBE_POSITION_Y);
	}
	else if (isTravelling && y > DOWN_TUBE_POSITION_Y + MARIO_BIG_BBOX_HEIGHT && !isAtMainWorld)
	{
		vy = 0;
		ay = MARIO_GRAVITY;	//Trả lại grav cho nó
		isTravelling = false;
		isTravelDown = false;
	}
	isAllowToUseTube = false;
}

void CMario::HandleTravellingUp()
{
	if (isTravelling && start_y - y > MARIO_BIG_BBOX_HEIGHT && !isAtMainWorld)
	{
		isAtMainWorld = true;
		SetPosition(SHORT_TUBE_POSITION_X, SHORT_TUBE_POSITION_Y);
	}
	else if (isTravelling && y < SHORT_TUBE_POSITION_Y - MARIO_BIG_BBOX_HEIGHT && isAtMainWorld)
	{
		vy = 0;
		ay = MARIO_GRAVITY;
		isTravelling = false;
		isTravelUp = false;
	}
	isAllowToUseTube = false;
}