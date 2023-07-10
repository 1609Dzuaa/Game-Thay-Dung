#include "Mario.h"
#include "Goomba.h"
#include "Coin.h"
#include "Brick.h"
#include "Koopa.h"
#include "QuestionBrick.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "ColorPlatform.h"
#include "Flower.h"
#include "FireBullet.h"
#include "Switch.h"
#include "EffectScore.h"
#include "EffectCollision.h"
#include "PlayScene.h"
#include "Collision.h"
#include "Tube.h"
#include "Card.h"

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt; //x = x0 + vx * dt
	y += vy * dt; //y = y0 + vy * dt
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
		OnCollisionWithBlockingObjects(e);
	//else //Va chạm với vật KHÔNG CÓ thuộc tính BLOCK HOẶC Question Brick
		OnCollisionWithNonBlockingObjects(e);
}

void CMario::OnCollisionWithBlockingObjects(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBrick*>(e->obj))
	{
		CBrick* br = dynamic_cast<CBrick*>(e->obj);
		if (br->GetType() == GOLD_BRICK)
		{
			if (br->GetState() != GBRICK_STATE_TURN_TO_COIN)
			{
				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny > 0)
					{
						//Nếu là viên chứa Item và state chưa bị hit
						if (br->GetItemType() != NO_ITEM && br->GetState() != GBRICK_HAS_ITEM_STATE_IS_HITTED)
						{
							br->SetSpeed(0, -GOLD_BRICK_BOUNCING_SPEED);
							br->SetHitted(true);
							br->SetState(GBRICK_HAS_ITEM_STATE_IS_HITTED);
						}
						else if (br->GetItemType() == NO_ITEM) //nếu 0 có Item
						{
							if (level > MARIO_LEVEL_SMALL) //Nếu level > small thì phá viên gạch luôn
								br->Delete();
							else
							{
								br->SetSpeed(0, -GOLD_BRICK_BOUNCING_SPEED);
								br->SetHitted(true);
							}
						}
					}
					else if (e->ny < 0)
					{
						//vy = 0;
						isOnPlatform = true;
						isLanding = false;
						CountJumpOnEnemies = 0; //Chạm đất thì reset số lần nhảy
						//SnappingToAnEdge(e, br);
						//vy = 0;
						//isOnPlatform = true;
					}
				}
				else if (e->nx != 0 && e->obj->IsBlocking())
				{
					vx = 0;
				}
			}
			else
			{
				br->Delete();
				if (coin == 99)
				{
					coin = 0;
					HP++;
				}
				else
					coin++;
				points += 50;
				//Tăng điểm cho Mario
			}
		}
		else if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
			if (e->ny < 0)
			{
				vy = 0; //bcuz of this
				isOnPlatform = true;
				CountJumpOnEnemies = 0; //Chạm đất thì reset số lần nhảy
			}
		}
		else if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}
	} //Nếu 0 phải Brick (Tube, quesbrick, Plat ...)
	else if (e->ny != 0 && e->obj->IsBlocking())
	{
		if (dynamic_cast<CTube*>(e->obj))
		{
			CTube* tube = dynamic_cast<CTube*>(e->obj);
			if (tube->GetType() == LONG_TUBE)
			{
				if (x > tube->GetX() - 7.0f && x < tube->GetX() + 7.0f)
					isAllowToUseTube = true;
				else
					isAllowToUseTube = false;

				vy = 0;
				isOnPlatform = true;
				CountJumpOnEnemies = 0;
				start_y = y + MARIO_BIG_BBOX_HEIGHT / 2;
			}
			else //Nếu nó là các loại ống còn lại
			{
				//vy = 0;
				isOnPlatform = true;
				CountJumpOnEnemies = 0;
			}
		}
		else if (dynamic_cast<CPlatform*>(e->obj))
		{
			CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);
			if (plat->GetType() == PLATFORM_UDW_TUBE)	//Nếu Platform đó là Ống chui lên
			{
				//Muốn Travel ngược về phải thoả 3ĐK:
				//1. Ở vị trí x cho phép
				//2. Combo phím Up và S được bấm
				//3. Va cột từ dưới lên (ny > 0)
				if (x > plat->GetX() + 5.5f && x < plat->GetX() + 15.0f)
					isAllowToUseTube = true;
				else
					isAllowToUseTube = false;

				if (e->ny > 0 && isAllowToUseTube && isComboUpAndS) 
				{
					start_y = y - MARIO_BIG_BBOX_HEIGHT / 2;
					isTravelUp = true;
					SetState(MARIO_STATE_TRAVELLING);
				}
			}
			else
			{
				vy = 0;
				SnappingToAnEdge(e, e->obj);
				isOnPlatform = true;
				CountJumpOnEnemies = 0;
			}
		}
		else
		{
			vy = 0;
			isOnPlatform = true;
			CountJumpOnEnemies = 0;
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
	if (dynamic_cast<CQuestionBrick*>(e->obj) && e->obj->GetState() != QBRICK_STATE_HITTED)
		OnCollisionWithQuesBrick(e);
	if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	if (dynamic_cast<CFlower*>(e->obj))
		OnCollisionWithFlower(e); 
	if (dynamic_cast<CFireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
	if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	if (dynamic_cast<CSwitch*>(e->obj))
		OnCollisionWithSwitch(e);
	if (dynamic_cast<CCard*>(e->obj))
		OnCollisionWithCard(e);
}

void CMario::OnCollisionWithColorPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		if (isEndGame)
		{
			CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
			HandleCollisionWithColorPlatform(e, cl_pf);
			CountJumpOnEnemies = 0;
			isOnPlatform = true;
			isLanding = false;
			SetState(MARIO_STATE_WALKING_RIGHT);
			return;
		}
		CColorPlatform* cl_pf = dynamic_cast<CColorPlatform*>(e->obj);
		HandleCollisionWithColorPlatform(e, cl_pf);
		CountJumpOnEnemies = 0;
		isOnPlatform = true;
		isLanding = false;
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
			}
		}
		else
		{
			if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 + 3.5 > color_platf->GetY())
			{
				this->y = color_platf->GetY() - MARIO_BIG_BBOX_HEIGHT / 2 - 3.5f; //trừ một lượng vừa đủ
				vy = 0;
			}
		}
	}
	else
	{
		if (this->y + MARIO_SMALL_BBOX_HEIGHT / 2 + color_platf->GetCellHeight() / 2 > color_platf->GetY())
		{
			this->y = color_platf->GetY() - color_platf->GetCellHeight() / 2 - MARIO_SMALL_BBOX_HEIGHT / 2;
			vy = 0;
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
			vy = -0.23f;
		}
		else if (goomba->GetType() == PARA_GOOMBA && goomba->GetLevel() == PARA_GOOMBA_LEVEL_NO_WINGS)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CountJumpOnEnemies++;
			SpawnScore(goomba);
			vy = -0.23f; //nảy lên
		}
		else //Goomba thường
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			CountJumpOnEnemies++;
			SpawnScore(goomba);
			vy = -0.23f; //nảy lên
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
			if (isHolding && e->nx != this->nx) //hướng va chạm phải khác hướng ôm rùa thì mới cho colli
			{
				goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
				SpawnScore(goomba);
				ghost_koopa->SetState(KOOPA_STATE_DIE);
				ghost_koopa->Delete();
				this->SetState(MARIO_STATE_IDLE);
				isHolding = false;
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
					SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	if (coin == 99)
	{
		coin = 0;
		HP++;
	}
	else
		coin++;
	points += 50;
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
			//Chỉ 1 trong 2 trạng thái này của Koopa mà Mario mới ôm đc
			if (isAllowToHoldKoopa && KoopaStateThatAllowToHold(koopa))
			{
				this->SetState(MARIO_STATE_HOLDING);
				this->isHolding = true;
				ghost_koopa = koopa; //turn on shield
				if (koopa->GetState() == KOOPA_STATE_SLEEP || koopa->GetState() == KOOPA_STATE_REBORN)
					koopa->SetState(KOOPA_STATE_BEING_HELD);
				else if (koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE || koopa->GetState() == KOOPA_STATE_REBORN_REVERSE
					|| koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
					koopa->SetState(KOOPA_STATE_BEING_HELD_REVERSE);
				if (nx > 0)
				{
					if (level > MARIO_LEVEL_SMALL)
						koopa->SetPosition(this->x + 11.5f, this->y + 1.5f);
					else
						koopa->SetPosition(this->x + 11.5f, this->y - 1.5f);
				}
				else
				{
					if (level > MARIO_LEVEL_SMALL)
						koopa->SetPosition(this->x - 11.5f, this->y + 1.5f);
					else
						koopa->SetPosition(this->x - 11.5f, this->y - 1.5f);
				}
			}
			else if (isHolding && e->nx != this->nx)
			{
				koopa->SetState(KOOPA_STATE_DIE);
				SpawnScore(koopa);
				ghost_koopa->SetState(KOOPA_STATE_DIE);
				ghost_koopa->Delete();
				this->SetState(MARIO_STATE_IDLE);
				isHolding = false;
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
						isHolding = false;
						this->SetState(MARIO_STATE_EVOLVING);
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				} //các state không gây dmg cho Mario
				else if (koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE
					|| koopa->GetState() == KOOPA_STATE_REBORN_REVERSE
					|| koopa->GetState() == KOOPA_STATE_SLEEP_REVERSE_SPECIAL)
				{
					if (nx > 0)
						this->SetState(MARIO_STATE_KICKING_RIGHT);
					else 
						this->SetState(MARIO_STATE_KICKING_LEFT);
					koopa->SetState(KOOPA_STATE_SLIP_REVERSE);
				}
				else //Đang Ngủ Bình Thường
				{
					if (nx > 0)
						this->SetState(MARIO_STATE_KICKING_RIGHT);
					else
						this->SetState(MARIO_STATE_KICKING_LEFT);
					koopa->SetState(KOOPA_STATE_SLIP);
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
	if (mr->GetType() == RED_MUSHROOM)
	{
		if (mr->GetState() != MUSHROOM_STATE_IN_THE_BRICK)
		{
			SpawnScore(mr);
			mr->Delete();
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2; //Giảm y để tránh tình trạng "Sink" dưới nền
			this->isEvolveForward = true;
			this->SetState(MARIO_STATE_EVOLVING);
			isAteItem = true;
		}
	}
	else
	{
		if (mr->GetState() != MUSHROOM_STATE_IN_THE_BRICK)
		{
			HP++;
			SpawnScore(mr); //Spawn Level Up
			mr->Delete();
		}
	}
	//points += 1000;
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
	//points += 1000;
}

void CMario::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
	if (!untouchable)
	{
		if (isHolding && e->nx != this->nx) //sometimes work ? Will check it later
		{
			e->obj->SetState(FLOWER_STATE_DIE);
			SpawnScore(e->obj);
			ghost_koopa->SetState(KOOPA_STATE_DIE);
			ghost_koopa->Delete();
			this->SetState(MARIO_STATE_IDLE);
			isHolding = false;
		}
		else if (level > MARIO_LEVEL_BIG) //Racoon thì spawn khói sau khi giảm level
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
			SetState(MARIO_STATE_DIE);
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
			SetState(MARIO_STATE_DIE);
	}
}

void CMario::OnCollisionWithSwitch(LPCOLLISIONEVENT e)
{
	if(e->ny < 0)
	if (e->obj->GetState() == SWITCH_STATE_NORMAL)
	{
		e->obj->SetState(SWITCH_STATE_HITTED);
		this->isHitSwitch = true;
		Shaking = true;
		shaking_start = GetTickCount64();
	}
}

void CMario::OnCollisionWithCard(LPCOLLISIONEVENT e)
{
	if (HasCollectCard) return; //Tránh TH ăn Card 2 lần liên tiếp trong cùng 1 Scene ??!

	//End game rồi nhưng phải chạm đất sau đó mới Auto Move!
	//Hiện card linh tinh là do 2 đoạn dưới @@, đã sửa
	CCard* card = dynamic_cast<CCard*>(e->obj);
	TypeOfCardCollected = card->GetType();	//Lấy loại card collect đc 
	HasCollectCard = true;
	if (TypeOfCardCollected == CARD_STATE_MUSHROOM)
		points += 1350;
	else if (TypeOfCardCollected == CARD_STATE_STAR)
		points += 1450;
	else 
		points += 1500;
	e->obj->SetState(CARD_STATE_BE_COLLECTED);
	isEndGame = true;
	CEffectScore* eff = new CEffectScore(COURSE_CLEAR_X, COURSE_CLEAR_Y, 0, COURSE_CLEAR_TEXT);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff);
}

void CMario::SnappingToAnEdge(LPCOLLISIONEVENT e, LPGAMEOBJECT obj)
{
	if (this->level != MARIO_LEVEL_SMALL) //level gấu mèo và BIG dùng chung BBox được vì diff 0 đáng kể
	{
		/*if (!isSitting) {
			if (gameobject->GetY() - GetY() < MARIO_BIG_BBOX_HEIGHT)
			{
				SetY(gameobject->GetY() - MARIO_BIG_BBOX_HEIGHT + 4);
				vy = 0;
				isOnPlatform = true;
			}
		}*/
		if (!isSitting)
		{
			if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 > obj->GetY() - 18.0f)
			{
				//SNAP: Player.Y = ColorPlat.Y - ColorPlat.Height / 2 - Player.Height / 2
				this->y = obj->GetY() - MARIO_BIG_BBOX_HEIGHT / 2 - 18.0f;
				//vy = 0.022f;		
				vy = 0;
				isOnPlatform = true;
			}
		}
		else
		{
			if (this->y + MARIO_BIG_BBOX_HEIGHT / 2 + 3.5 > obj->GetY())
			{
				this->y = obj->GetY() - MARIO_BIG_BBOX_HEIGHT / 2 - 3.5f; //trừ một lượng vừa đủ
				vy = 0;
				isOnPlatform = true;
			}
		}
	}
	else
	{
		if (this->y + MARIO_SMALL_BBOX_HEIGHT / 2 > obj->GetY())
		{
			this->y = obj->GetY() - MARIO_SMALL_BBOX_HEIGHT / 2;
			vy = 0;
			isOnPlatform = true;
		}
	}
}