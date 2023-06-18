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
#include "ColorPlatform.h"
#include "ShootingFlower.h"
#include "FireBullet.h"
#include "EffectScore.h"
#include "EffectCollision.h"
#include "PlayScene.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Phải dừng vận tốc ở đây chứ 0 phải ở hàm OnNoCollision
	//Vì ở đây gọi trc r mới đến OnNoCollision

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
	//UpdateTail here
	if (tail != NULL)
		UpdateTailPosition(tail);

	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Pos Y: %f", y);
}

void CMario::UpdateMarioState()
{
	//May be affect collision with Fire Bullet
	// reset untouchable timer if untouchable time has passed
	//when untouchable, there are 2 states of drawing: Draw & NOT draw
	HandleUntouchableDrawState();

	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
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
			else if(this->level == MARIO_LEVEL_BIG)
				this->SetLevel(MARIO_LEVEL_SMALL);
		}
		isEvolving = false;
		isEvolveForward = false;
		isEvolveBackward = false;
		isAteItem = false;
		evolve_start = 0;
	}

	if (this->isJumping && this->level == MARIO_LEVEL_RACOON && this->vy >= 0)
	{
		this->SetState(MARIO_RACOON_STATE_FALLING);
		//Khi đang bay ở trạng thái Racoon và vy đổi dấu
		//thì bật hoạt ảnh falling lên
	}

	if (this->state == MARIO_STATE_RUNNING_RIGHT || this->state == MARIO_STATE_WALKING_RIGHT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_RIGHT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_VX_RIGHT);
		}
	}
	else if (this->state == MARIO_STATE_RUNNING_LEFT || this->state == MARIO_STATE_WALKING_LEFT)
	{
		if (abs(vx) > abs(maxVx))
		{
			vx = maxVx;
			if (this->state == MARIO_STATE_RUNNING_LEFT)
				this->SetState(MARIO_STATE_RUNNING_AT_MAX_VX_LEFT);
		}
	}
	else if (this->isAtMaxSpeed && isJumping)
		this->SetState(MARIO_STATE_JUMP_AT_MAX_SPEED);

	if (isKicking && GetTickCount64() - kick_start >= MARIO_KICK_TIME)
	{
		isKicking = false;
		kick_start = 0;
	}

	if (isAttacking && GetTickCount64() - attack_start >= MARIO_RACOON_ATTACK_TIME)
	{
		isAttacking = false;
		attack_start = 0;
	}
}

void CMario::HandleUntouchableDrawState()
{
	if (untouchable && GetTickCount64() - untouch_draw_0 >= UNTOUCH_DRAW_TIME && untouch_0)
	{
		untouch_0 = 0;
		untouch_1 = 1;
		untouch_draw_0 = 0;
		untouch_draw_1 = GetTickCount64();
	}
	else if (untouchable && GetTickCount64() - untouch_draw_1 >= UNTOUCH_DRAW_TIME && untouch_1)
	{
		untouch_0 = 1;
		untouch_1 = 0;
		untouch_draw_0 = GetTickCount64();
		untouch_draw_1 = 0;
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt; //x = x0 + vx * dt
	y += vy * dt; //y = y0 + vy * dt
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	OnCollisionWithBlockingObjects(e);

	//Va chạm với vật KHÔNG CÓ thuộc tính BLOCK HOẶC Question Brick
	OnCollisionWithNonBlockingObjects(e);
}

void CMario::OnCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isOnPlatform = true;
			CountJumpOnEnemies = 0; //Chạm đất thì reset số lần nhảy
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
}

void CMario::OnCollisionWithNonBlockingObjects(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CColorPlatform*>(e->obj))
		OnCollisionWithColorPlatform(e);
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	if (dynamic_cast<CQuestionBrick*>(e->obj) && e->obj->GetState() != QBRICK_STATE_HITTED) //chưa bị đụng thì mới có collision
		OnCollisionWithQuesBrick(e);
	if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	if (dynamic_cast<CShootingFlower*>(e->obj))
		OnCollisionWithFlower(e);
	if (dynamic_cast<CFireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
	if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
}

void CMario::OnCollisionWithColorPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
		CountJumpOnEnemies = 0;
	}
}

void CMario::HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf)
{
	//Hàm này để xử lý vị trí của Mario khi nhảy lên Color Platform
	//Nhằm tránh việc nó bị rơi xuống dù có va chạm
	//Tương tự như hàm SetLevel trong framework của thầy.

	//Tham khảo mục "Snapping to an Edge" của bài viết: 
	// https://happycoding.io/tutorials/processing/collision-detection
	//"we want the position of the bottom of the player to equal the position of the top of the ground. 
	//And since our player’s position is usually stored as its top Y value and a height, 
	//that means we want the position of the top of our player to equal the ground’s top minus the player’s height.":
	//Condition that I learned: |Player.TopY + Player.Height > ColorPlat.TopY => SNAP|
	if (this->level != MARIO_LEVEL_SMALL) //level gấu mèo và BIG dùng chung BBox được vì diff 0 đáng kể
	{
		if (!isSitting)
		{
			if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
			{
				//SNAP: Player.Y = ColorPlat.Y - ColorPlat.Height / 2 - Player.Height / 2
				this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
				vy = 0;
				isOnPlatform = true;
			}
		}
		else
		{
			if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 + 3.5 > color_platf->GetY())
			{
				this->y = color_platf->GetY() - MARIO_BIG_BBOX_HEIGHT / 2 - 3.5f; //trừ một lượng vừa đủ
				vy = 0;
				isOnPlatform = true;
			}
		}
	}
	else
	{
		if (this->y + MARIO_SMALL_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
		{
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - MARIO_SMALL_BBOX_HEIGHT / 2;
			vy = 0;
			isOnPlatform = true;
		}
	}
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
		if (goomba->GetType() == PARA_GOOMBA && goomba->GetLevel() == PARA_GOOMBA_LEVEL_HAS_WINGS)
		{
			goomba->SetLevel(PARA_GOOMBA_LEVEL_NO_WINGS);
			CountJumpOnEnemies++;
			SpawnScore(goomba);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (goomba->GetType() == PARA_GOOMBA && goomba->GetLevel() == PARA_GOOMBA_LEVEL_NO_WINGS)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CountJumpOnEnemies++;
			SpawnScore(goomba);
			vy = -MARIO_JUMP_DEFLECT_SPEED; //nảy lên
		}
		else //Goomba thường
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CountJumpOnEnemies++;
			SpawnScore(goomba);
			vy = -MARIO_JUMP_DEFLECT_SPEED; //nảy lên
		}
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
				SpawnScore(goomba);
				SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
				//tail->SetState(TAIL_STATE_ATTACK);
				//tail->OnCollisionWithGoomba(e);
			}
			else
			{
				if (level > MARIO_LEVEL_BIG) //Racoon thì spawn khói sau khi giảm level
				{
					this->isEvolveBackward = true;
					this->SetState(MARIO_STATE_EVOLVING);
					SpawnEffect(e, this, EFF_COL_TYPE_SMOKE_EVOLVE);
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					this->isEvolveBackward = true;
					this->SetState(MARIO_STATE_EVOLVING);
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
		if (koopa->GetType() == GREEN_FLYING_KOOPA)
		{
			koopa->SetType(GREEN_KOOPA); //biến nó thành Koopa thường
			koopa->SetState(KOOPA_STATE_WALKING);
			CountJumpOnEnemies++;
			SpawnScore(koopa);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else //Koopa thường
		{
			if (koopa->GetState() == KOOPA_STATE_SLIP)
			{
				koopa->SetState(KOOPA_STATE_SLEEP);
				CountJumpOnEnemies++;
				SpawnScore(koopa);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopa->GetState() == KOOPA_STATE_SLIP_REVERSE)
			{
				koopa->SetState(KOOPA_STATE_SLEEP_REVERSE_SPECIAL);
				CountJumpOnEnemies++;
				SpawnScore(koopa);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopa->GetState() == KOOPA_STATE_SLEEP) //Koopa ĐANG NGỦ
			{
				CountJumpOnEnemies++;
				SpawnScore(koopa);
				koopa->SetState(KOOPA_STATE_SLIP);
			}
			else if (koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE
				|| koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL
				|| koopa->GetState() == KOOPA_STATE_REBORN_REVERSE)
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
			else //Trạng thái đi bộ vuốt râu
			{
				koopa->SetState(KOOPA_STATE_SLEEP);
				CountJumpOnEnemies++;
				SpawnScore(koopa);
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
			if (this->isAttacking)
			{
				if (koopa->GetType() == GREEN_FLYING_KOOPA)
				{
					koopa->SetType(GREEN_KOOPA);
					koopa->SetState(KOOPA_STATE_SLEEP_REVERSE);
					SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
				}
				else
				{
					koopa->SetState(KOOPA_STATE_SLEEP_REVERSE);
					SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
				}
			}
			else
			{
				//4 state của Koopa mà gây dmg lên Mario
				if (koopa->GetState() == KOOPA_STATE_WALKING || 
					koopa->GetState() == KOOPA_STATE_SLIP ||
					koopa->GetState() == KOOPA_STATE_SLIP_REVERSE || 
					koopa->GetState() == KOOPA_STATE_JUMPING)
				{
					if (level > MARIO_LEVEL_BIG) //Racoon thì spawn khói sau khi giảm level
					{
						this->isEvolveBackward = true;
						this->SetState(MARIO_STATE_EVOLVING);
						SpawnEffect(e, this, EFF_COL_TYPE_SMOKE_EVOLVE);
						StartUntouchable();
					}
					else if (level > MARIO_LEVEL_SMALL)
					{
						this->isEvolveBackward = true;
						this->SetState(MARIO_STATE_EVOLVING);
						StartUntouchable();
					}
					else
					{
						SetState(MARIO_STATE_DIE);
					}
				} //các state không gây dmg cho Mario
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
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);

	if (e->ny == 1)
		qb->SetState(QBRICK_STATE_HITTED);

	//rơi tiền, nấm ra
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mr = dynamic_cast<CMushroom*>(e->obj);
	if (mr->GetState() != MUSHROOM_STATE_IN_THE_BRICK)
	{
		SpawnScore(mr);
		mr->Delete();
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2; //Giảm y để tránh tình trạng "Sink" dưới nền
		this->isEvolveForward = true;
		this->SetState(MARIO_STATE_EVOLVING);
		isAteItem = true;
	}
	//Da Fuq Mario blocking Mushroom ??
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (this->level == MARIO_LEVEL_SMALL)
	{
		this->isEvolveForward = true;
		this->SetState(MARIO_STATE_EVOLVING);
	}
	else if (this->level == MARIO_LEVEL_BIG)
	{
		this->isEvolveForward = true;
		this->SetState(MARIO_STATE_EVOLVING);
		SpawnEffect(e, leaf, EFF_COL_TYPE_SMOKE_EVOLVE);
	}
	SpawnScore(leaf);
	leaf->Delete();
}

void CMario::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
	if (isAttacking)
	{
		e->obj->SetState(SHOOTING_FLOWER_STATE_DIE);
		SpawnScore(e->obj);
		SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
	}
	else 
	{
		if (!untouchable)
		{
			if (level > MARIO_LEVEL_BIG) //Racoon thì spawn khói sau khi giảm level
			{
				this->isEvolveBackward = true;
				this->SetState(MARIO_STATE_EVOLVING);
				SpawnEffect(e, this, EFF_COL_TYPE_SMOKE_EVOLVE);
				StartUntouchable();
			}
			else if (level > MARIO_LEVEL_SMALL)
			{
				this->isEvolveBackward = true;
				this->SetState(MARIO_STATE_EVOLVING);
				StartUntouchable();
			}
			else
			{
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
	if (!untouchable)
	{
		if (level > MARIO_LEVEL_BIG) //Racoon thì spawn khói sau khi giảm level
		{
			this->isEvolveBackward = true;
			this->SetState(MARIO_STATE_EVOLVING);
			SpawnEffect(e, this, EFF_COL_TYPE_SMOKE_EVOLVE);
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			this->isEvolveBackward = true;
			this->SetState(MARIO_STATE_EVOLVING);
			StartUntouchable();
		}
		else
		{
			SetState(MARIO_STATE_DIE);
		}
	}
}

//
// Get animation ID for small Mario
//

int CMario::GetAniIdSmall()
{
	int aniId = -1;

	if (!isOnPlatform)
	{
		if (isEvolving && isEvolveForward)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
		}
		else if (isAtMaxSpeed)
		{
			if (nx > 0)  //prob here
				aniId = ID_ANI_MARIO_SMALL_JUMP_AT_MAX_SPEED_RIGHT;
			else if (nx < 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_AT_MAX_SPEED_LEFT;
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
		else if (isEvolving && isEvolveForward)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
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
				else if (vx >= maxRunningSpeed)  //Đặt thằng này lên trc vì lúc chạy ax = accel run (obviously!)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_MAX_SPEED_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (abs(vx) >= maxRunningSpeed)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_MAX_SPEED_LEFT;
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
		else if (isAtMaxSpeed)
		{
			if (nx > 0)  //prob here
				aniId = ID_ANI_MARIO_BIG_JUMP_AT_MAX_SPEED_RIGHT;
			else if (nx < 0)
				aniId = ID_ANI_MARIO_BIG_JUMP_AT_MAX_SPEED_LEFT;
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
		else if (isEvolving && isEvolveBackward) //tiến hoá lùi
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT;
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
				else if (isAtMaxSpeed)  //prob here
					aniId = ID_ANI_MARIO_BIG_RUNNING_MAX_SPEED_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (isAtMaxSpeed)
					aniId = ID_ANI_MARIO_BIG_RUNNING_MAX_SPEED_LEFT;
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
	if (!isOnPlatform) //included fly, jump, landing, fall
	{
		if (vy > 0 && nx < 0 && isLanding)
		{
			aniId = ID_ANI_MARIO_RACOON_LANDING_LEFT;
		}
		else if (vy > 0 && nx > 0 && isLanding)
		{
			aniId = ID_ANI_MARIO_RACOON_LANDING_RIGHT;
		}
		else if (vy > 0 && nx < 0)
		{
			aniId = ID_ANI_MARIO_RACOON_FALLING_LEFT;
		}
		else if (vy > 0 && nx > 0)
		{
			aniId = ID_ANI_MARIO_RACOON_FALLING_RIGHT;
		}
		else
		{
			if (isAtMaxSpeed)
			{
				if (nx > 0)  //prob here
					aniId = ID_ANI_MARIO_RACOON_JUMP_AT_MAX_SPEED_RIGHT;
				else if (nx < 0)
					aniId = ID_ANI_MARIO_RACOON_JUMP_AT_MAX_SPEED_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT;
			}
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
				else if (vx >= maxRunningSpeed)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_MAX_SPEED_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_RIGHT;
				else if (abs(vx) >= maxRunningSpeed)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_MAX_SPEED_LEFT;
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
	if (isEvolving && level == MARIO_LEVEL_BIG && isEvolveForward) return; //Big biến thành gấu mèo thì 0 vẽ trong 1 khoảng thgian
	if (isEvolving && level == MARIO_LEVEL_RACOON && isEvolveBackward) return; //Gấu mèo biến về Big thì cũng 0 vẽ
	if (untouchable && untouch_0) return;

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
	//RenderBoundingBox();
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

	case MARIO_STATE_RUNNING_AT_MAX_VX_RIGHT:
		if (isSitting) break;
		isAtMaxSpeed = true;
		if (this->level == MARIO_LEVEL_RACOON)
			canFly = true;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;

	case MARIO_STATE_RUNNING_AT_MAX_VX_LEFT:
		if (isSitting) break;
		isAtMaxSpeed = true;
		if (this->level == MARIO_LEVEL_RACOON)
			canFly = true;
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
			isJumping = true;
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_JUMP_AT_MAX_SPEED:
		if (isSitting) break;
		if (isOnPlatform)
		{
			isJumping = true;
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
			break;
		}

	case MARIO_RACOON_STATE_FALLING:
	{
		isJumping = false;
		isLanding = false;
		break;
	}

	case MARIO_RACOON_STATE_LANDING:
	{
		//isLanding = true;
		vy = 0.0005f;
		break;
	}

	case MARIO_RACOON_STATE_FLYING:
	{

		break;
	}

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		isJumping = false;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
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
		attack_start = GetTickCount64();
		isAttacking = true;
		isKicking = false;
		isSitting = false;
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_EVOLVING:
		evolve_start = GetTickCount64();
		isEvolving = true;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED * 1.5f;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
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
	//DebugOutTitle(L"y, l, t, r, b: %f, %f, %f, %f, %f, %f", y, left, top, right, bottom);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	/*if (l == MARIO_LEVEL_RACOON) //nếu là level Racoon thì check và tạo đuôi
	{
		if (tail != NULL) return; //đã có đuôi thì không tạo nữa

		tail = new CTail(x, y, MARIO_BIG_BBOX_WIDTH, this->nx);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		current_scene->AddObjectToScene(tail);
		DebugOut(L"Tail was created and add to object list\n");
	}
	else 
	{
		/*if (tail != NULL)
		{
			CTail* temp_tail = tail;
			tail = NULL;
			delete temp_tail;
			CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			current_scene->GetObjectsVector().erase(std::remove(current_scene->GetObjectsVector().begin(), current_scene->GetObjectsVector().end(), tail), current_scene->GetObjectsVector().end());
			DebugOut(L"Delete Tail success!\n");
		}
	}*/

	if(!isAteItem) //Nếu đc SetLevel bằng cách nhấn phím thì giảm y để Mario kh bị rơi xuống nền
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
		eff_scr = new CEffectScore(obj->GetX(), obj->GetY() - 15.0f, obj->GetY() - 45.0f, ITEM_SCORE);

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

void CMario::UpdateTailPosition(CTail* tail)
{
	if (this->nx > 0)
		tail->SetPosition(this->x - MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT / 6 + 1.5f);
	else
		tail->SetPosition(this->x + MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT / 6 + 1.5f);
}