#include "SMB3Curtain.h"
#include "Mushroom.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Leaf.h"
#include "StarIntro.h"
#include "IntroPlayScene.h"

void CSMB3Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Đi quá mốc nào đó thì Spawn quái
	if (y > 80 && !HasSpawned)
	{
		SpawnOtherObjects();
		vy = 0;
		HasSpawned = true;
	}

	//DebugOutTitle(L"Y: %f", this->y);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSMB3Curtain::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CSMB3Curtain::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_SMB3_CURTAIN)->Render(x, y, false);
}

void CSMB3Curtain::SetState(int state)
{
	
}

void CSMB3Curtain::SpawnOtherObjects()
{
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CGoomba* goomba = new CGoomba(50, -100);
	CMushroom* mr = new CMushroom(45, -100, 0, RED_MUSHROOM);
	CKoopa* green_koopa = new CKoopa(120, -200, 1, 0);
	CKoopa* black_koopa = new CKoopa(178, -115, 0, 1);
	CLeaf* leaf = new CLeaf(130, 0, 1);
	CStarIntro* star = new CStarIntro(185, -60);
	mr->SetState(MUSHROOM_STATE_OUT_OF_BRICK);
	mr->SetSpeed(-MUSHROOM_SPEED_X, 0);
	current_scene->AddObjectToScene(goomba);
	current_scene->AddObjectToScene(mr);
	current_scene->AddObjectToScene(green_koopa);
	current_scene->AddObjectToScene(black_koopa);
	current_scene->AddObjectToScene(leaf);
	current_scene->AddObjectToScene(star);
}