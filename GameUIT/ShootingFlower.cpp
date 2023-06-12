#include "ShootingFlower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "debug.h"

void CShootingFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y <= minY)
	{
		this->SetState(SHOOTING_FLOWER_STATE_ATTACK);
		this->minY = y;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	
	DebugOutTitle(L"Flower Y: %f", y);
	//SHOULD ADD RIGHT LEFT LOGIC TO FLOWER HERE
}

void CShootingFlower::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CShootingFlower::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (GetAniID() == -1);
	else
		animations->Get(GetAniID())->Render(x, y);
}

int CShootingFlower::GetAniID()
{
	int aniID = -1;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == SHOOTING_FLOWER_STATE_OUT_OF_TUBE)
	{
		if (mario->GetX() > this->x)
			aniID = ID_ANI_FLOWER_RISE_UP_RIGHT;
		else if(mario->GetX() < this->x)
			aniID = ID_ANI_FLOWER_RISE_UP_LEFT;
	}
	else if (state == SHOOTING_FLOWER_STATE_DIVE)
	{
		if (mario->GetX() > this->x)
			aniID = ID_ANI_FLOWER_DIVE_RIGHT;
		else if (mario->GetX() < this->x)
			aniID = ID_ANI_FLOWER_DIVE_LEFT;
	}
	else if (state == SHOOTING_FLOWER_STATE_ATTACK)
	{
		if (mario->GetX() > this->x)
			aniID = ID_ANI_FLOWER_IDLE_RIGHT;
		else if (mario->GetX() < this->x)
			aniID = ID_ANI_FLOWER_IDLE_LEFT;
	}

	return aniID;
}

void CShootingFlower::SetState(int state)
{
	switch (state)
	{
	case SHOOTING_FLOWER_STATE_DIVE:
		vy = SHOOTING_FLOWER_RISE_SPEED;

		break;

	case SHOOTING_FLOWER_STATE_OUT_OF_TUBE:
		vy = -SHOOTING_FLOWER_RISE_SPEED;

		break;

	case SHOOTING_FLOWER_STATE_ATTACK:
		vy = 0;

		break;
	}

	CGameObject::SetState(state);
}

void CShootingFlower::AimAndShoot()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CFireBullet* fire_bullet = new CFireBullet(this->x, this->y);
	fire_bullet->SetPosition(this->x, this->y);
	if (this->x <= mario->GetX())
		fire_bullet->SetVX(FIRE_BULLET_VX);
	else 
		fire_bullet->SetVX(-FIRE_BULLET_VX);

	current_scene->AddObjectToScene(fire_bullet);

	SetState(SHOOTING_FLOWER_STATE_DIVE);
}

void CShootingFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FLOWER_WIDTH / 2;
	t = y - FLOWER_HEIGHT / 2;
	r = l + FLOWER_WIDTH;
	b = t + FLOWER_HEIGHT;
}