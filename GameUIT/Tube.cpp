#include "Tube.h"
#include "PlayScene.h"
#include "ShootingFlower.h"
#include "Camera.h"

void CTube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Nếu Mario ở trong tầm tấn công thì trồi lên
	if (!CCamera::GetInstance()->isViewable(this)) return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();	
	if (mario->GetX() >= this->x - ATTACK_ZONE && mario->GetX() < this->x - SAFE_ZONE && !shoot_flower->IsDeleted()
		|| mario->GetX() > SAFE_ZONE + this->x && mario->GetX() <= this->x + ATTACK_ZONE && !shoot_flower->IsDeleted())
		SpawnShootingFlower(); //chia ra 2 vùng bên trái phải
}

void CTube::SpawnShootingFlower()
{
	if (shoot_flower->GetState() == SHOOTING_FLOWER_STATE_IN_TUBE) //chỉ khi đang ở trong Tube thì mới trỗi dậy
		this->shoot_flower->SetState(SHOOTING_FLOWER_STATE_RISE_UP);
}

void CTube::Render()
{
	if (!CCamera::GetInstance()->isViewable(this)) return;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_TUBE)->Render(x, y, false);
	//RenderBoundingBox();
}

void CTube::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TUBE_BBOX_WIDTH / 2;
	top = y - TUBE_BBOX_HEIGHT / 2;
	right = left + TUBE_BBOX_WIDTH;
	bottom = top + TUBE_BBOX_HEIGHT;
}