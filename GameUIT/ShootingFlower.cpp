#include "ShootingFlower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "debug.h"

void CShootingFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (state == SHOOTING_FLOWER_STATE_SHOOT)
		AimAndShoot();*/
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetStopWatch()) return;

	if (state == SHOOTING_FLOWER_STATE_REST && GetTickCount64() - rest_start > SHOOTING_FLOWER_REST_TIME)
	{
		enableToShoot = true;
		rest_start = 0;
	}

	if (state == SHOOTING_FLOWER_STATE_SHOOT)// && enableToShoot)
		AimAndShoot();

	if (y <= minY && state == SHOOTING_FLOWER_STATE_RISE_UP)
	{
		this->SetState(SHOOTING_FLOWER_STATE_SHOOT); //chui lên cao nhất thì tấn công 
		this->y = minY;
	}

	if (y >= maxY)
	{
		this->SetState(SHOOTING_FLOWER_STATE_IN_TUBE); //chui xuống thấp nhất thì ngưng vẽ, cập nhật
		this->y = maxY;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CShootingFlower::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CShootingFlower::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int animation_ID = GetAniID();

	if (animation_ID != -1)
		animations->Get(animation_ID)->Render(x, y, true);
}

int CShootingFlower::GetAniID()
{
	int aniID = -1;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == SHOOTING_FLOWER_STATE_RISE_UP) //trỗi dậy
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
	else if (state == SHOOTING_FLOWER_STATE_SHOOT)
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
	else if (state == SHOOTING_FLOWER_STATE_DIVE) //chui xuống
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

void CShootingFlower::SetState(int state)
{
	if (this->state == SHOOTING_FLOWER_STATE_DIE) return;

	switch (state)
	{
	case SHOOTING_FLOWER_STATE_DIVE:
		vy = SHOOTING_FLOWER_RISE_SPEED;
		aim_start = 0;
		rest_start = GetTickCount64();

		break;

	case SHOOTING_FLOWER_STATE_RISE_UP:
		vy = -SHOOTING_FLOWER_RISE_SPEED;

		break;

	case SHOOTING_FLOWER_STATE_SHOOT:
		vy = 0;
		aim_start = GetTickCount64();

		break;

	case SHOOTING_FLOWER_STATE_REST:
		vy = 0;
		enableToShoot = false;

		break;

	case SHOOTING_FLOWER_STATE_DIE: //Cây sinh effect khi chết
		this->Delete();
		HandleFlowerDeadEffect();

		break;
	}

	CGameObject::SetState(state);
}

void CShootingFlower::HandleFlowerDeadEffect()
{
	die_start = GetTickCount64();
	eff_die = new CEffectCollision(x, y - FLOWER_HEIGHT / 2 + 3.0f, die_start, EFF_COL_TYPE_SMOKE_EVOLVE);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff_die);
}

void CShootingFlower::AimAndShoot()
{
	if (GetTickCount64() - aim_start >= SHOOTING_FLOWER_AIM_TIME)
	{
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		CFireBullet* fire_bullet = new CFireBullet(this->x, this->y);
		fire_bullet->SetPosition(this->x, this->y - 2.0f);
		SetSpeedFireBullet(fire_bullet, mario);
		current_scene->AddObjectToScene(fire_bullet);

		SetState(SHOOTING_FLOWER_STATE_DIVE);
	}
}

void CShootingFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FLOWER_WIDTH / 2;
	t = y - FLOWER_HEIGHT / 2;
	r = l + FLOWER_WIDTH;
	b = t + FLOWER_HEIGHT;
}

CFireBullet* CShootingFlower::SetSpeedFireBullet(CFireBullet* bullet, CMario* mario)
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