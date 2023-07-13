#include "SMB3Curtain.h"
#include "Mushroom.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Leaf.h"
#include "StarIntro.h"
#include "NB3Intro.h"
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
	if (state == SMB3_STATE_BLACK_BG)
		animations->Get(ID_ANI_SMB3_CURTAIN_BLACK_BG)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_SMB3_CURTAIN_COLOR_BG)->Render(x, y, false);
}

void CSMB3Curtain::SetState(int state)
{
	this->state = state;
}

void CSMB3Curtain::SpawnOtherObjects()
{
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CGoomba* goomba = new CGoomba(70, -100);
	CMushroom* mr = new CMushroom(45, -100, 0, RED_MUSHROOM);
	CKoopa* green_koopa = new CKoopa(120, -200, 1, 0);
	CKoopa* black_koopa = new CKoopa(178, -115, 0, 1);
	CLeaf* leaf = new CLeaf(130, 0, 1);
	CStarIntro* star = new CStarIntro(185, -60);
	CNb3Racoon* nb3 = new CNb3Racoon(130.0f, 120.0f);
	mr->SetState(MUSHROOM_STATE_OUT_OF_BRICK);
	mr->SetSpeed(-MUSHROOM_SPEED_X, 0);
	current_scene->AddObjectToScene(goomba, 0);
	current_scene->AddObjectToScene(mr, 0);
	current_scene->AddObjectToScene(green_koopa, 0);
	current_scene->AddObjectToScene(black_koopa, 0);
	current_scene->AddObjectToScene(leaf, 0);
	current_scene->AddObjectToScene(star, 0);
	current_scene->AddObjectToScene(nb3, 2);
	SetPosition(125, 115);
	SetState(SMB3_STATE_COLOR_BG);
}