#include "ShootingFlower.h"
#include "Mario.h"
#include "PlayScene.h"

CShootingFlower::CShootingFlower()
{
	this->state = SHOOTING_FLOWER_STATE_IN_TUBE;
}

void CShootingFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (this->x - mario->GetMarioPositionX() <= ATTACK_RANGE)
		this->SetState(SHOOTING_FLOWER_STATE_RISE_UP);
	//Nếu Mario ở trong tầm tấn công thì trồi lên

	//SHOULD ADD RIGHT LEFT LOGIC TO FLOWER HERE
}

void CShootingFlower::Render()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	if (state == SHOOTING_FLOWER_STATE_RISE_UP)
	{
		if (mario->GetMarioPositionX() > this->x)
			animations->Get(ID_ANI_FLOWER_RISE_UP_RIGHT)->Render(x, y);
		else 
			animations->Get(ID_ANI_FLOWER_RISE_UP_LEFT)->Render(x, y);
	}
	else if (state == SHOOTING_FLOWER_STATE_IDLE)
	{
		if (mario->GetMarioPositionX() > this->x)
			animations->Get(ID_ANI_FLOWER_IDLE_RIGHT)->Render(x, y);
		else
			animations->Get(ID_ANI_FLOWER_IDLE_LEFT)->Render(x, y);
	}
}

void CShootingFlower::SetState(int state)
{
	switch (state)
	{
	case SHOOTING_FLOWER_STATE_RISE_UP:

		break;

	case SHOOTING_FLOWER_STATE_IDLE:

		break;
	}
}

void CShootingFlower::Shoot()
{

}