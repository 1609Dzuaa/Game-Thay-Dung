#include "ShootingFlower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "debug.h"

CShootingFlower::CShootingFlower(float x, float y)
{
	this->x = x;
	this->y = y;
	this->vy = SHOOTING_FLOWER_RISE_SPEED;
	this->state = SHOOTING_FLOWER_STATE_IN_TUBE;
	//minY = y - FLOWER_HEIGHT / 2; Finding Why initialize value in constructor failed??
}

void CShootingFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != SHOOTING_FLOWER_STATE_OUT_OF_TUBE) //dive || rise
	{
		if (state == SHOOTING_FLOWER_STATE_IN_TUBE)
		{
			if (y <= minY)
			{
				y = minY;
				this->SetState(SHOOTING_FLOWER_STATE_OUT_OF_TUBE); //Idle
			}
		}
		else
		{
			if (y >= minY)
			{
				this->SetState(SHOOTING_FLOWER_STATE_REST);
			}
		}
	}
	else if(state == SHOOTING_FLOWER_STATE_OUT_OF_TUBE)
	{
		Shoot();
	}
	else 
	{
		vy += ay * dt;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	
	DebugOutTitle(L"Flower Y: %f", y);
	//SHOULD ADD RIGHT LEFT LOGIC TO FLOWER HERE
}

void CShootingFlower::OnNoCollision(DWORD dt)
{
	if (state != SHOOTING_FLOWER_STATE_IN_TUBE)
	{
		if (state == SHOOTING_FLOWER_STATE_DIVE)
			y += vy * dt;
	}
	else
	{
		y += -vy * dt; //Nếu nó đang ở trong ống thì trỗi dậy
	}
}

void CShootingFlower::Render()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	if (state == SHOOTING_FLOWER_STATE_IN_TUBE)
	{
		if (mario->GetMarioPositionX() >= this->x)
			animations->Get(ID_ANI_FLOWER_RISE_UP_RIGHT)->Render(x, y);
		else 
			animations->Get(ID_ANI_FLOWER_RISE_UP_LEFT)->Render(x, y);
	}
	else if (state == SHOOTING_FLOWER_STATE_OUT_OF_TUBE)
	{
		if (mario->GetMarioPositionX() >= this->x)
			animations->Get(ID_ANI_FLOWER_IDLE_RIGHT)->Render(x, y);
		else
			animations->Get(ID_ANI_FLOWER_IDLE_LEFT)->Render(x, y);
	}
	else if (state == SHOOTING_FLOWER_STATE_REST)
	{
		//NO RENDER
	}
	else
	{
		if (mario->GetMarioPositionX() > this->x)
			animations->Get(ID_ANI_FLOWER_DIVE_RIGHT)->Render(x, y);
		else
			animations->Get(ID_ANI_FLOWER_DIVE_LEFT)->Render(x, y);
	}
}

void CShootingFlower::SetState(int state)
{
	switch (state)
	{
	case SHOOTING_FLOWER_STATE_DIVE:
		vy = SHOOTING_FLOWER_RISE_SPEED;

		break;

	case SHOOTING_FLOWER_STATE_OUT_OF_TUBE:
		vy = 0;

		break;

	case SHOOTING_FLOWER_STATE_REST:

		break;
	}

	CGameObject::SetState(state);
}

void CShootingFlower::Shoot()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CFireBullet* fire_bullet = new CFireBullet(this->x, this->y);
	fire_bullet->SetPosition(this->x, this->y);
	if (this->x <= mario->GetMarioPositionX())
		fire_bullet->SetVX(FIRE_BULLET_VX);
	else 
		fire_bullet->SetVX(-FIRE_BULLET_VX);

	current_scene->AddObjectToScene(fire_bullet);

	//có vấn đề với đoạn mã trên

	SetState(SHOOTING_FLOWER_STATE_DIVE);
}

void CShootingFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FLOWER_WIDTH / 2;
	t = y - FLOWER_HEIGHT / 2;
	r = l + FLOWER_WIDTH;
	b = t + FLOWER_HEIGHT;
}