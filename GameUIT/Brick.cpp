#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
}


void CFireBullet::Update(DWORD dt)
{
	this->x += vx * dt;
}

void CFireBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BULLET)->Render(x, y);
}

void CFireBullet::SetState(int state)
{
	switch (state)
	{
	case BULLET_STATE_SHOOT:
		this->x = 200.0f;
		this->y = 155.0f;
	}
	//Set trạng thái cho viên đạn
}