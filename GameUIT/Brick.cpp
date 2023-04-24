#include "Brick.h"
#include "Mario.h"

extern CMario* mario;
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

void CFireBullet::Fire(int para)
{
	//0: right, 1:left
	//Hàm này nhận tham số để quyết định bắn sang phải hay trái
	switch (para)
	{
	case 0:
		this->vx = abs(vx);
		this->x = mario->GetX() + 5.0f;
		this->y = mario->GetY() + 0.0f;
		break;
	case 1:
		if (this->vx > 0)
			this->vx = -this->vx;
		this->x = mario->GetX() - 5.0f;
		this->y = mario->GetY() + 0.0f;
		break;
	}
}