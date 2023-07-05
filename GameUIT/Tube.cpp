#include "Tube.h"
#include "PlayScene.h"
#include "Flower.h"
#include "Camera.h"

void CTube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Nếu Mario ở trong tầm tấn công thì trồi lên
	if (!CCamera::GetInstance()->isViewable(this)) return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (Type == TUBE_HAS_RED_FLOWER || Type == TUBE_HAS_GREEN_FLOWER)
	{
		if (mario->GetX() >= this->x - ATTACK_ZONE && mario->GetX() < this->x - SAFE_ZONE && !flower->IsDeleted()
			|| mario->GetX() > SAFE_ZONE + this->x && mario->GetX() <= this->x + ATTACK_ZONE && !flower->IsDeleted())
			SpawnFlower(); //chia ra 2 vùng bên trái phải
	}
	else if(Type == TUBE_HAS_GREEN_CARNIVOURING_FLOWER)
	{
		if (mario->GetX() >= this->x - SAFE_ZONE_CARNI_FLOWER
			&& this->x + SAFE_ZONE_CARNI_FLOWER >= mario->GetX() && !flower->IsDeleted())
			return;// || this->x + SAFE_ZONE_CARNI_FLOWER >= mario->GetX()) return;

		SpawnFlower();
	}
}

void CTube::SpawnFlower()
{
	if (flower->GetState() == FLOWER_STATE_IN_TUBE) //chỉ khi đang ở trong Tube thì mới trỗi dậy
		this->flower->SetState(FLOWER_STATE_RISE_UP);
}

void CTube::Render()
{
	if (!CCamera::GetInstance()->isViewable(this))
		return;

	CAnimations* animations = CAnimations::GetInstance();
	if (Type == TUBE_HAS_GREEN_CARNIVOURING_FLOWER || Type == SHORT_TUBE_HAS_NO_FLOWER)
		animations->Get(ID_ANI_SMALL_TUBE)->Render(x, y, false);
	else if(Type == TUBE_HAS_2_STRIPE_BRICK)
		animations->Get(ID_ANI_TUBE_WITH_STRIPE_BRICK)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_TUBE)->Render(x, y, false);
	//RenderBoundingBox();
}

void CTube::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (Type == TUBE_HAS_GREEN_CARNIVOURING_FLOWER || Type == SHORT_TUBE_HAS_NO_FLOWER)
	{
		left = x - TUBE_BBOX_WIDTH / 2;
		top = y - TUBE_CARNI_BBOX_HEIGHT / 2;
		right = left + TUBE_BBOX_WIDTH;
		bottom = top + TUBE_CARNI_BBOX_HEIGHT;
	}
	else
	{
		left = x - TUBE_BBOX_WIDTH / 2;
		top = y - TUBE_BBOX_HEIGHT / 2;
		right = left + TUBE_BBOX_WIDTH;
		bottom = top + TUBE_BBOX_HEIGHT;
	}
}