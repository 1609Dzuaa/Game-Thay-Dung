#include "Flower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "Tail.h"
#include "debug.h"
#include "DataBinding.h"

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	/*if (state == FLOWER_STATE_SHOOT)
		AimAndShoot();*/
	if (CDataBindings::GetInstance()->IsStopWatch) return;

	if (type == FLOWER_TYPE_SHOOTING_RED)
		UpdateShootinFlower();
	else if (type == FLOWER_TYPE_CARNIVOURING_GREEN)
		UpdateGreenCarniFlower();
	else if (type == FLOWER_TYPE_SHOOTING_GREEN)
		UpdateShootinFlower();

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"y, vy: %f, %f", y, vy);
}

void CFlower::UpdateShootinFlower()
{
	if (state == FLOWER_STATE_REST && GetTickCount64() - rest_start > FLOWER_REST_TIME)
	{
		enableToShoot = true;
		rest_start = 0;
	}

	if (state == FLOWER_STATE_ATTACK)// && enableToShoot)
		AimAndShoot();

	if (y <= minY && state == FLOWER_STATE_RISE_UP)
	{
		this->SetState(FLOWER_STATE_ATTACK); //chui lên cao nhất thì tấn công 
		this->y = minY;
	}

	if (y >= maxY)
	{
		this->SetState(FLOWER_STATE_IN_TUBE); //chui xuống thấp nhất thì ngưng vẽ, cập nhật
		this->y = maxY;
	}
}

void CFlower::UpdateGreenCarniFlower()
{
	if (state == FLOWER_STATE_ATTACK)// && enableToShoot)
		AimAndShoot();

	if (y <= minY && state == FLOWER_STATE_RISE_UP)
	{
		this->SetState(FLOWER_STATE_ATTACK); 
		this->y = minY;
	}

	if (y >= maxY)
	{
		this->SetState(FLOWER_STATE_IN_TUBE); //chui xuống thấp nhất thì ngưng vẽ, cập nhật
		this->y = maxY;
	}
}

void CFlower::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

/*void CFlower::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) return;
}*/

void CFlower::Render()
{
	if (IsWaiting && IsWaitable) return;

	CAnimations* animations = CAnimations::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int animation_ID = -1;
	if (type == FLOWER_TYPE_SHOOTING_RED)
		animation_ID = GetAniID_Red();
	else if (type == FLOWER_TYPE_CARNIVOURING_GREEN)
		animation_ID = GetAniID_Carni();
	else if (type == FLOWER_TYPE_SHOOTING_GREEN)
		animation_ID = GetAniID_Green();

	if (animation_ID != -1)
		animations->Get(animation_ID)->Render(x, y, true);
	//RenderBoundingBox();
}

int CFlower::GetAniID_Red()
{
	int aniID = -1;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == FLOWER_STATE_RISE_UP) //trỗi dậy
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_RISE_UP_RIGHT;
		else if(mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_RISE_UP_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_RISE_UP_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_RISE_UP_RIGHT_FACE_UP;
	}
	else if (state == FLOWER_STATE_ATTACK)
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_SHOOT_RIGHT;
		else if (mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_SHOOT_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_SHOOT_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_SHOOT_RIGHT_FACE_UP;
	}
	else if (state == FLOWER_STATE_DIVE) //chui xuống
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_DIVE_RIGHT;
		else if (mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_FLOWER_DIVE_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_DIVE_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_FLOWER_DIVE_RIGHT_FACE_UP;
	}

	return aniID;
}

int CFlower::GetAniID_Carni()
{
	return ID_ANI_FLOWER_CARNIVOUR_RISE;
}

int CFlower::GetAniID_Green()
{
	int aniID = -1;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == FLOWER_STATE_RISE_UP) //trỗi dậy
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_RISE_UP_RIGHT;
		else if (mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_RISE_UP_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_RISE_UP_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_RISE_UP_RIGHT_FACE_UP;
	}
	else if (state == FLOWER_STATE_ATTACK)
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_SHOOT_RIGHT;
		else if (mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_SHOOT_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_SHOOT_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_SHOOT_RIGHT_FACE_UP;
	}
	else if (state == FLOWER_STATE_DIVE) //chui xuống
	{
		if (mario->GetX() > this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_DIVE_RIGHT;
		else if (mario->GetX() < this->x && mario->GetY() > this->y)
			aniID = ID_ANI_GREEN_FLOWER_DIVE_LEFT;
		else if (mario->GetX() < this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_DIVE_LEFT_FACE_UP;
		else if (mario->GetX() > this->x && mario->GetY() <= this->y)
			aniID = ID_ANI_GREEN_FLOWER_DIVE_RIGHT_FACE_UP;
	}

	return aniID;
}

void CFlower::SetState(int state)
{
	if (this->state == FLOWER_STATE_DIE) return;

	switch (state)
	{
	case FLOWER_STATE_DIVE:
		vy = FLOWER_RISE_SPEED;
		aim_start = 0;
		rest_start = GetTickCount64();

		break;

	case FLOWER_STATE_RISE_UP:
		vy = -FLOWER_RISE_SPEED;
		//ay = FLOWER_GRAVITY;

		break;

	case FLOWER_STATE_ATTACK:
		vy = 0;
		aim_start = GetTickCount64();

		break;

	case FLOWER_STATE_REST:
		vy = 0;
		enableToShoot = false;

		break;

	case FLOWER_STATE_DIE: //Cây sinh effect khi chết
		this->Delete();
		HandleFlowerDeadEffect();

		break;
	}

	CGameObject::SetState(state);
}

void CFlower::HandleFlowerDeadEffect()
{
	die_start = GetTickCount64();
	eff_die = new CEffectCollision(x, y - FLOWER_HEIGHT / 2 + 3.0f, die_start, EFF_COL_TYPE_SMOKE_EVOLVE);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff_die);
}

void CFlower::AimAndShoot()
{
	if (GetTickCount64() - aim_start >= FLOWER_AIM_TIME)
	{
		if (type != FLOWER_TYPE_CARNIVOURING_GREEN)
		{
			CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			CFireBullet* fire_bullet = new CFireBullet(this->x, this->y);
			fire_bullet->SetPosition(this->x, this->y - 2.0f);
			SetSpeedFireBullet(fire_bullet, mario);
			current_scene->AddObjectToScene(fire_bullet);
		}
		SetState(FLOWER_STATE_DIVE);
	}
}

void CFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == FLOWER_TYPE_SHOOTING_RED)
	{
		l = x - FLOWER_WIDTH / 2;
		t = y - FLOWER_HEIGHT / 2;
		r = l + FLOWER_WIDTH;
		b = t + FLOWER_HEIGHT;
	}
	else if(type == FLOWER_TYPE_SHOOTING_GREEN || type == FLOWER_TYPE_CARNIVOURING_GREEN)
	{
		l = x - FLOWER_WIDTH / 2;
		t = y - GREEN_FLOWER_HEIGHT / 2;
		r = l + FLOWER_WIDTH;
		b = t + GREEN_FLOWER_HEIGHT;
	}
}

CFireBullet* CFlower::SetSpeedFireBullet(CFireBullet* bullet, CMario* mario)
{
	if (this->x <= mario->GetX() && this->y < mario->GetY())
		bullet->SetSpeed(FIRE_BULLET_VX, FIRE_BULLET_VY);
	else if (this->x > mario->GetX() && this->y > mario->GetY())
		bullet->SetSpeed(-FIRE_BULLET_VX, -FIRE_BULLET_VY);
	else if (this->x > mario->GetX() && this->y < mario->GetY())
		bullet->SetSpeed(-FIRE_BULLET_VX, FIRE_BULLET_VY);
	else if (this->x <= mario->GetX() && this->y > mario->GetY())
		bullet->SetSpeed(FIRE_BULLET_VX, -FIRE_BULLET_VY);

	return bullet;
}