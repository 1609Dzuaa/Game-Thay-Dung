#include "Tube.h"
#include "PlayScene.h"
#include "ShootingFlower.h"

void CTube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Nếu Mario ở trong tầm tấn công thì trồi lên
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetMarioPositionX() >= this->x - ATTACK_ZONE)
		SpawnShootingFlower();
}

void CTube::SpawnShootingFlower()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CShootingFlower* flower = new CShootingFlower(this->x, this->y);
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