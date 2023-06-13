#include "Tube.h"
#include "PlayScene.h"
#include "ShootingFlower.h"

void CTube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Nếu Mario ở trong tầm tấn công thì trồi lên
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() >= this->x - ATTACK_ZONE && !shoot_flower->IsDeleted())
		 SpawnShootingFlower(); //Tạm để đây xem lại sau
}

void CTube::SpawnShootingFlower()
{
	if (shoot_flower->GetState() == SHOOTING_FLOWER_STATE_IN_TUBE) //chỉ khi đang ở trong Tube thì mới trỗi dậy
		this->shoot_flower->SetState(SHOOTING_FLOWER_STATE_OUT_OF_TUBE);
	//problem here
	//Khi Mario trong tầm nguy hiểm thì hàm này sẽ bị gọi nhiều lần dẫn đến 
	//việc hoa luôn trong tình trạng out_of_tube
}

void CTube::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_TUBE)->Render(x, y);
	//RenderBoundingBox();
}

void CTube::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TUBE_BBOX_WIDTH / 2;
	top = y - TUBE_BBOX_HEIGHT / 2;
	right = left + TUBE_BBOX_WIDTH;
	bottom = top + TUBE_BBOX_HEIGHT;
}

void CTube::SetState(int state)
{
	switch (state)
	{
	case TUBE_STATE_STOP_SPAWN:
		Flower_Arise = true;
		break;
	}

	CGameObject::SetState(state);
}