#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Brick.h"
#include "Koopa.h"
#include "QuestionBrick.h"
#include "Mushroom.h"
#include "Leaf.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	//Maybe use a function and call it here

	if (isKicking && GetTickCount64() - kick_start >= MARIO_KICK_TIME)
	{
		isKicking = false;
		kick_start = 0;
	}

	if (isAttacking && GetTickCount64() - attack_start >= MARIO_RACOON_ATTACK_TIME)
	{
		isAttacking = false;
		kick_start = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt; //x = x0 + vx * dt
	y += vy * dt; //y = y0 + vy * dt
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isOnPlatform = true;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0; 
	}

	//Va chạm với vật không có thuộc tính block hoặc Question Brick
	KindOfOnCollisionWith(e);
}

void CMario::KindOfOnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	if (dynamic_cast<CQuestionBrick*>(e->obj) && e->obj->GetState() != QBRICK_STATE_HITTED)
		OnCollisionWithQuesBrick(e);
	if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->ny < 0)
		HandleCollisionUpperDirectionWithGoomba(goomba);
	else
		HandleCollisionOtherDirectionWithGoomba(e, goomba);
}

void CMario::HandleCollisionUpperDirectionWithGoomba(CGoomba* goomba)
{
	if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
		vy = -MARIO_JUMP_DEFLECT_SPEED; //nảy lên
	}
}

void CMario::HandleCollisionOtherDirectionWithGoomba(LPCOLLISIONEVENT e, CGoomba* goomba)
{
	if (untouchable == 0)
	{
		//Nếu con Goomba chưa chết thì xét, không thì bỏ qua nó
		if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_REVERSE)
		{
			if (this->isAttacking)
			{
				goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
			}
			else
			{
				if (level > MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (e->ny < 0)
		HandleCollisionUpperDirectionWithKoopa(koopa);
	else
		HandleCollisionOtherDirectionWithKoopa(e, koopa);
}

void CMario::HandleCollisionUpperDirectionWithKoopa(CKoopa* koopa)
{
	//Va chạm Hướng TRÊN (UP) => đưa Koopa về trạng thái ngủ/lăn
	if (koopa->GetState() != KOOPA_STATE_DIE)
	{
		if (koopa->GetType() == 2)
		{
			koopa->SetType(1);
			koopa->SetState(KOOPA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else
		{
			if (koopa->GetState() == KOOPA_STATE_SLIP)
			{
				koopa->SetState(KOOPA_STATE_SLEEP);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopa->GetState() == KOOPA_STATE_SLIP_REVERSE)
			{
				koopa->SetState(KOOPA_STATE_SLEEP_REVERSE_SPECIAL);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if(koopa->GetState() == KOOPA_STATE_SLEEP) //Koopa ĐANG NGỦ
			{
				//Dựa vào hướng của Mario để quyết định dấu vận tốc của Koopa
				if (this->nx > 0)
				{
					koopa->SetState(KOOPA_STATE_SLIP);
				}
				else if (this->nx < 0)
				{
					koopa->SetState(KOOPA_STATE_SLIP);
				}
			}
			else if (koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE || koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
			{
				if (this->nx > 0)
				{
					koopa->SetState(KOOPA_STATE_SLIP_REVERSE);
				}
				else if (this->nx < 0)
				{
					koopa->SetState(KOOPA_STATE_SLIP_REVERSE);
				}
			}
			else
			{
				koopa->SetState(KOOPA_STATE_SLEEP);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
}

void CMario::HandleCollisionOtherDirectionWithKoopa(LPCOLLISIONEVENT e, CKoopa* koopa)
{
	//Va chạm các hướng KHÁC hướng TRÊN
	if (untouchable == 0) //can be touched
	{
		if (koopa->GetState() != KOOPA_STATE_DIE)
		{
			if (this->isAttacking) //thêm check if tấn công vào koopa đang ngủ
			{
				koopa->SetState(KOOPA_STATE_SLEEP_REVERSE);
			}
			else 
			{
				if (koopa->GetState() != KOOPA_STATE_SLEEP && koopa->GetState() != KOOPA_STATE_SLEEP_REVERSE && koopa->GetState() != KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
				{
					if (level == MARIO_LEVEL_RACOON)
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
					else if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						SetState(MARIO_STATE_DIE);
					}
				}
				else if (koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE 
					|| koopa->GetState() == KOOPA_STATE_REBORN_REVERSE 
					|| koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
				{
					if (e->nx == -1)
					{
						this->SetState(MARIO_STATE_KICKING_RIGHT);
						koopa->SetState(KOOPA_STATE_SLIP_REVERSE);
					}
					else if (e->nx == 1)
					{
						this->SetState(MARIO_STATE_KICKING_LEFT);
						koopa->SetState(KOOPA_STATE_SLIP_REVERSE);
					}
				}
				else //Đang Ngủ Bình Thường
				{
					if (e->nx == -1)
					{
						this->SetState(MARIO_STATE_KICKING_RIGHT);
						koopa->SetState(KOOPA_STATE_SLIP);
					}
					else if (e->nx == 1)
					{
						this->SetState(MARIO_STATE_KICKING_LEFT);
						koopa->SetState(KOOPA_STATE_SLIP);
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithQuesBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick *qb = dynamic_cast<CQuestionBrick*>(e->obj);

	if (e->ny == 1)
		qb->SetState(QBRICK_STATE_HITTED);

	//rơi tiền, nấm ra
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mr = dynamic_cast<CMushroom*>(e->obj);

	mr->Delete();

	this->SetLevel(2);

	//Da Fuq Mario blocking Mushroom ??
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	this->SetLevel(MARIO_LEVEL_RACOON);
	leaf->Delete();
}

//
// Get animation ID for small Mario
//

int CMario::GetAniIdSmall()
{
	int aniId = -1;

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
	{
		if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_KICKING_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for big Mario
//

int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
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
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_KICKING_LEFT;
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

//
//Id for Racoon Mario
//

int CMario::GetAniIdRacoon()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT;
		}
	}
	else
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_KICKING_LEFT;
		}
		else if (isAttacking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_ATTACKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_ATTACKING_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_LEFT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_RIGHT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_LEFT;
			}
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;

	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
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

	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform) 
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level == MARIO_LEVEL_BIG)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		else if (isOnPlatform && level == MARIO_LEVEL_RACOON)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_RACOON_STATE_ATTACK:
		if (isSitting) break;
		isAttacking = true;
		isKicking = false;
		isSitting = false;
		attack_start = GetTickCount64();
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED * 1.5;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Really IMPORTANT
	if (level == MARIO_LEVEL_BIG)
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
	else if (level == MARIO_LEVEL_RACOON)
	{
		if (isSitting)
		{
			left = x - MARIO_RACOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	//DebugOutTitle(L"x, y, l, t, r, b: %f, %f, %f, %f, %f, %f", x, y, left, top, right, bottom);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else
	{
		//y -= (MARIO_BIG_BBOX_HEIGHT) / 2;
	}
	level = l;
}

