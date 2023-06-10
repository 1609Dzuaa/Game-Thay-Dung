#include "Tube.h"
#include "PlayScene.h"
#include "ShootingFlower.h"

void CTube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Nếu Mario ở trong tầm tấn công thì trồi lên
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetMarioPositionX() >= this->x - ATTACK_ZONE && !Flower_Arise) //ở gần VÀ ở trong tầm tấn công
		 SpawnShootingFlower();
}

void CTube::SpawnShootingFlower()
{
	this->Flower_Arise = true;
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CShootingFlower* flower = new CShootingFlower(this->x, this->y - TUBE_BBOX_HEIGHT / 2);
	flower->SetFlowerMinY(this->y - TUBE_BBOX_HEIGHT / 2 - FLOWER_HEIGHT / 2);
	current_scene->AddObjectToScene(flower);
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